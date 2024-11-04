import socket
import random
import time
import queue
import threading

SERVER_HOST = 'localhost'
SERVER_PORT = 65432
GENERATION_MIN, GENERATION_MAX = 100, 200  
PACKET_DROP_PROB = 0.1      
DELAY_MIN =50
DELAY_MAX = 150  
SEQ_MODULO = 8    
WINDOW_SIZE = 7    
TOTAL_PACKETS = 10000  
packet_queue = queue.Queue()

def generate_packets():
    for i in range(TOTAL_PACKETS):
        time.sleep(random.uniform(GENERATION_MIN / 1000.0, GENERATION_MAX / 1000.0)) 
        packet_queue.put(f"Packet {i}")
        print(f"Generated {packet_queue.queue[-1]}")

def send_packet(connection, frame, sequence_number):
    print(f"Sending {frame} (Seq: {sequence_number})")
    connection.sendall(f"{frame} (Seq: {sequence_number})".encode())

def handle_acks(connection, base, next_seq):
    while base < next_seq:
        try:
            ack = connection.recv(1024).decode()
            print(f"Received ACK for {ack}")
            base = int(ack) + 1  
        except socket.timeout:
            print("Timeout, no ACK received, resending from base")
            next_seq = base  
        except Exception as e:
            print(f"Error receiving ACK: {e}")
            break
    return base

def send_packets(connection):
    base = 0
    next_seq = 0

    while base < TOTAL_PACKETS:
        while next_seq < base + WINDOW_SIZE and not packet_queue.empty():
            frame = packet_queue.get()
            seq_num = next_seq % SEQ_MODULO
            if random.random() < PACKET_DROP_PROB:
                print(f"Dropped {frame} (Seq: {seq_num})")
                next_seq += 1  
                continue
            send_packet(connection, frame, seq_num)
            next_seq += 1

        base = handle_acks(connection, base, next_seq)

def run_client():
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
            client_socket.settimeout(1.0)  
            client_socket.connect((SERVER_HOST, SERVER_PORT))
            generator_thread = threading.Thread(target=generate_packets)
            generator_thread.start()
            send_packets(client_socket)
            generator_thread.join()
    except Exception as e:
        print(f"Client error: {e}")

def main():
    run_client()

main()
