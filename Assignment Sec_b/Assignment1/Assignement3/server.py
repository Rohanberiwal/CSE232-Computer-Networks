import socket
import random
import time

HOST = 'localhost'
PORT = 65432
P_DROP = 0.1       
T3, T4 = 50, 150  
SEQ_MODULO = 8     
NUM_PACKETS = 10000  

def go_back_n_receiver(conn):
    expected_seq = 0

    while expected_seq < NUM_PACKETS:
        try:
            frame = conn.recv(1024).decode()
            if not frame:
                print("Connection closed by the client.")
                break
            
            print(f"Received {frame}")
            if random.random() < P_DROP:
                print(f"Dropped ACK for {expected_seq}")
                continue
            
            print(f"Sending ACK for {expected_seq}")
            ack = str(expected_seq).encode()
            time.sleep(random.uniform(T3 / 1000.0, T4 / 1000.0))  
            conn.sendall(ack)
            expected_seq += 1

        except ConnectionResetError:
            print("Connection reset by peer.")
            break
        except socket.error as e:
            print(f"Socket error occurred: {e}")
            break
        except Exception as e:
            print(f"Unexpected error: {e}")
            break

def run_server():
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((HOST, PORT))
            s.listen()
            print("Server started on port", PORT)
            conn, addr = s.accept()
            with conn:
                print("Connected by", addr)
                go_back_n_receiver(conn)
    except OSError as e:
        print(f"Socket error: {e}")
    except Exception as e:
        print(f"Unexpected error while running server: {e}")

run_server()
