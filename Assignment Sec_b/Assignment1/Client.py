import socket
import time

# Configuration
SERVER_IP = '127.0.0.1'
SERVER_PORT = 12000
TIMEOUT = 10 # Timeout in seconds
PING_COUNT = 10

def create_udp_client():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.settimeout(TIMEOUT)
    return client_socket

def ping_server(client_socket, server_address, sequence_number):
    message = f"Ping {sequence_number} {time.time()}"
    start_time = time.time()
    
    try:
        # Send the ping message
        client_socket.sendto(message.encode(), server_address)
        
        # Receive the response
        response, _ = client_socket.recvfrom(1024)
        end_time = time.time()
        
        # Calculate RTT
        rtt = end_time - start_time
        print(f"Reply from server: {response.decode()} RTT = {rtt:.4f} seconds")
        return rtt
    except socket.timeout:
        print("Request timed out")
        return None

def main():
    server_address = (SERVER_IP, SERVER_PORT)
    client_socket = create_udp_client()
    
    rtts = []
    packet_loss_count = 0
    
    for i in range(1, PING_COUNT + 1):
        print(f"Sending ping {i}...")
        rtt = ping_server(client_socket, server_address, i)
        if rtt is not None:
            rtts.append(rtt)
        else:
            packet_loss_count += 1
        time.sleep(1) 
    client_socket.close()
    
    if rtts:
        print(f"\n--- Ping statistics ---")
        print(f"Packets sent: {PING_COUNT}")
        print(f"Packets received: {len(rtts)}")
        print(f"Packet loss: {packet_loss_count / PING_COUNT * 100:.2f}%")
        
        print(f"RTT min/avg/max = {min(rtts):.4f}/{sum(rtts) / len(rtts):.4f}/{max(rtts):.4f} seconds")
    else:
        print("All packets were lost.")

if __name__ == "__main__":
    main()
