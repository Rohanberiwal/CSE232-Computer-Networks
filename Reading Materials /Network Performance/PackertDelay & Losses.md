# Network Delays and Processing

## Overview

This README provides an overview of the different types of delays encountered in network communication, including processing delay, nodal processing delay, transmission delay, propagation delay, and queuing delay. Understanding these delays is crucial for analyzing and optimizing network performance.

## Types of Network Delays

### 1. **Processing Delay**

- **Definition:**
  - Processing delay refers to the time taken by a network device (e.g., router, switch) to process a packet. This includes tasks such as error checking, packet validation, and routing decisions.

- **Components:**
  - **Header Inspection:** Analyzing the packet header to determine where to forward the packet.
  - **Error Checking:** Checking for errors in packet headers or data.

- **Formula:**
  - Processing delay is generally not directly formulaic and is influenced by the device's processing power and efficiency.

### 2. **Nodal Processing Delay**

- **Definition:**
  - Nodal processing delay is the time taken for a network node (router or switch) to handle the packet before forwarding it to the next node.

- **Components:**
  - **Routing:** Determining the best path for the packet.
  - **Buffering:** Temporarily storing the packet if necessary.

- **Formula:**
  - Typically measured in milliseconds and varies based on the network device's performance.

### 3. **Transmission Delay**

- **Definition:**
  - Transmission delay is the time required to push all the packet's bits onto the link. It depends on the packet size and the link's bandwidth.

- **Formula:**
  \[
  \text{Transmission Delay} = \frac{\text{Packet Size (Bits)}}{\text{Bandwidth (bps)}}
  \]

- **Example:**
  - For a 1,000-bit packet and a 1 Mbps link:
    \[
    \text{Transmission Delay} = \frac{1{,}000 \text{ Bits}}{1{,}000{,}000 \text{ bps}} = 0.001 \text{ seconds} = 1 \text{ millisecond}
    \]

### 4. **Propagation Delay**

- **Definition:**
  - Propagation delay is the time taken for a signal to travel from the sender to the receiver over the physical medium. It is dependent on the distance between the sender and receiver and the speed of the signal in the medium.

- **Formula:**
  \[
  \text{Propagation Delay} = \frac{\text{Distance}}{\text{Propagation Speed}}
  \]
  - **Distance** is the length of the link.
  - **Propagation Speed** is the speed at which the signal travels through the medium (e.g., speed of light in fiber optics).

- **Example:**
  - For a 1,000 km link with a signal speed of \(2 \times 10^8\) meters per second:
    \[
    \text{Propagation Delay} = \frac{1{,}000{,}000 \text{ meters}}{2 \times 10^8 \text{ meters/second}} = 0.005 \text{ seconds} = 5 \text{ milliseconds}
    \]

### 5. **Queuing Delay**

- **Definition:**
  - Queuing delay is the time a packet spends waiting in a queue before it can be transmitted. This delay depends on the traffic load and the scheduling policies of the network device.

- **Formula:**
  - Queuing delay is highly variable and depends on the current traffic load and the network device's buffer management. There is no simple formula, but it can be estimated using queueing models like **Little’s Law**:
    \[
    \text{Average Queuing Delay} = \frac{\text{Average Number of Packets in Queue}}{\text{Average Packet Arrival Rate}}
    \]

## Summary of Delays

1. **Processing Delay:** Time to handle and process the packet within the node.
2. **Nodal Processing Delay:** Specific processing time within a network node.
3. **Transmission Delay:** Time to push all packet bits onto the link.
4. **Propagation Delay:** Time for the signal to travel through the medium.
5. **Queuing Delay:** Time a packet spends waiting in a queue.

## Python Code for Delay Calculations

```python
def transmission_delay(packet_size_bits, bandwidth_bps):
    return packet_size_bits / bandwidth_bps

def propagation_delay(distance_meters, propagation_speed_mps):
    return distance_meters / propagation_speed_mps

def queuing_delay(avg_number_of_packets, arrival_rate_packets_per_second):
    return avg_number_of_packets / arrival_rate_packets_per_second

# Example usage
packet_size_bits = 1_000
bandwidth_bps = 1_000_000
distance_meters = 1_000_000
propagation_speed_mps = 2e8
avg_number_of_packets = 10
arrival_rate_packets_per_second = 100

trans_delay = transmission_delay(packet_size_bits, bandwidth_bps)
prop_delay = propagation_delay(distance_meters, propagation_speed_mps)
queue_delay = queuing_delay(avg_number_of_packets, arrival_rate_packets_per_second)

print(f"Transmission Delay: {trans_delay:.3f} seconds")
print(f"Propagation Delay: {prop_delay:.3f} seconds")
print(f"Queuing Delay: {queue_delay:.3f} seconds")
