import socket
import threading
import random
import time

# Server parameters
HOST = 'localhost'
PORT = 65432
P_DROP = 0.1  # Probability of dropping ACKs
T3, T4 = 10, 50  # Delay range in ms

def go_back_n_receiver(conn):
    expected_seq = 0

    while expected_seq < 10000:
        # Simulate receiving frame
        time.sleep(random.uniform(T3 / 1000.0, T4 / 1000.0))  # Delay before receiving
        frame = conn.recv(1024).decode()  # Receive frame
        
        if not frame:
            break  # Exit on empty frame

        if random.random() < P_DROP:
            print(f"Dropped ACK for {frame}")
            continue  # Simulate dropped ACK
        else:
            print(f"Received {frame}")
            # Send acknowledgment
            ack = str(expected_seq).encode()
            conn.sendall(ack)
            expected_seq += 1  # Move to next expected sequence

def run_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        print("Server listening...")
        conn, addr = s.accept()
        with conn:
            print("Connected by", addr)
            go_back_n_receiver(conn)

# Start the server
run_server()
