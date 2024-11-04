import socket
import threading
import random
import time

# Client parameters
HOST = 'localhost'
PORT = 65432
T1, T2 = 50, 150  # Packet generation time range in ms
P_DROP = 0.1  # Probability of dropping frames
WINDOW_SIZE = 7  # Go-Back-N window size
SEQ_MODULO = 8  # Sequence number modulo
NUM_PACKETS = 10000  # Total packets to send

def packet_generator(conn):
    """Simulates packet generation and sending."""
    base = 0
    next_seq = 0

    while base < NUM_PACKETS:
        # Send packets in the window
        while next_seq < base + WINDOW_SIZE and next_seq < NUM_PACKETS:
            # Simulate frame encapsulation
            frame = f"Frame {next_seq % SEQ_MODULO}"
            # Randomly decide to drop the frame
            if random.random() < P_DROP:
                print(f"Dropped {frame}")
            else:
                print(f"Sending {frame}")
                conn.sendall(frame.encode())  # Send frame
                time.sleep(random.uniform(T1 / 1000.0, T2 / 1000.0))  # Wait before sending next frame
            next_seq += 1

        # Wait for acknowledgment
        while base < next_seq:
            try:
                ack = conn.recv(1024).decode()  # Wait for ACK
                print(f"Received ACK for {ack}")
                base += 1
            except socket.timeout:
                break  # Timeout, no ACK received

def run_client():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        packet_generator(s)

# Start the client
run_client()
