# Network Performance Formulas

## Overview

This README provides formulas used to analyze and calculate various aspects of network performance, including transmission time, throughput, and related metrics.

## Key Concepts and Formulas

### 1. **File Size Conversion**

- **Kilobytes (KB) to Bits:**
  \[
  \text{Bits} = \text{KB} \times 1{,}024 \text{ Bytes/KB} \times 8 \text{ Bits/Byte}
  \]
- **Bytes to Bits:**
  \[
  \text{Bits} = \text{Bytes} \times 8
  \]

### 2. **Transmission Time**

- **Formula:**
  \[
  \text{Transmission Time} = \frac{\text{File Size (Bits)}}{\text{Bandwidth (bps)}}
  \]
- **Where:**
  - **File Size** is in bits.
  - **Bandwidth** is in bits per second (bps).

### 3. **Throughput**

- **Throughput Calculation:**
  \[
  \text{Throughput} = \frac{\text{File Size (Bits)}}{\text{Transmission Time}}
  \]
- **Throughput and Bandwidth Relationship:**
  - Throughput typically matches the bottleneck bandwidth under ideal conditions with no congestion or packet loss.

### 4. **Round-Trip Time (RTT)**

- **RTT Calculation:**
  \[
  \text{RTT} = 2 \times \text{One-Way Delay} + \text{Transmission Time} + \text{ACK Time}
  \]
- **Where:**
  - **One-Way Delay** is the time taken for a packet to travel from sender to receiver.
  - **ACK Time** is the time taken for an acknowledgment to return to the sender.

### 5. **Effective Bandwidth**

- **Effective Bandwidth Calculation:**
  \[
  \text{Effective Bandwidth} = \frac{\text{File Size (Bits)}}{\text{RTT}}
  \]
- **Effective Bandwidth** takes into account the time required for acknowledgments and delays in the network.

## Example Calculations

### **Example 1: Transmission Time for a 125 KB File**

1. **File Size:** 125 KB
2. **Bandwidth:** 1 Mbps

**Convert 125 KB to Bits:**
\[
\text{Bits} = 125 \text{ KB} \times 1{,}024 \text{ Bytes/KB} \times 8 \text{ Bits/Byte} = 1{,}024{,}000 \text{ Bits}
\]

**Calculate Transmission Time:**
\[
\text{Transmission Time} = \frac{1{,}024{,}000 \text{ Bits}}{1{,}000{,}000 \text{ bps}} = 1.024 \text{ seconds}
\]

### **Example 2: Throughput Calculation**

**Given:**
- **File Size:** 1 MB (Megabyte)
- **Transmission Time:** 8 seconds

**Convert 1 MB to Bits:**
\[
\text{Bits} = 1 \text{ MB} \times 1{,}024 \text{ KB/MB} \times 1{,}024 \text{ Bytes/KB} \times 8 \text{ Bits/Byte} = 8{,}388{,}608 \text{ Bits}
\]

**Calculate Throughput:**
\[
\text{Throughput} = \frac{8{,}388{,}608 \text{ Bits}}{8 \text{ seconds}} = 1{,}048{,}576 \text{ bps} \approx 1.048 \text{ Mbps}
\]

## Python Code for Network Performance Calculations

```python
def file_size_to_bits(file_size_kb):
    return file_size_kb * 1024 * 8

def calculate_transmission_time(file_size_bits, bandwidth_bps):
    return file_size_bits / bandwidth_bps

def calculate_throughput(file_size_bits, transmission_time_s):
    return file_size_bits / transmission_time_s

def calculate_rtt(one_way_delay_ms, ack_time_ms, transmission_time_ms):
    return 2 * one_way_delay_ms + transmission_time_ms + ack_time_ms

def calculate_effective_bandwidth(file_size_bits, rtt_ms):
    return file_size_bits / (rtt_ms / 1000)

# Example usage
file_size_kb = 125
bandwidth_mbps = 1
one_way_delay_ms = 20
ack_time_ms = 20

file_size_bits = file_size_to_bits(file_size_kb)
bandwidth_bps = bandwidth_mbps * 1_000_000

transmission_time_s = calculate_transmission_time(file_size_bits, bandwidth_bps)
rtt_ms = calculate_rtt(one_way_delay_ms, ack_time_ms, transmission_time_s * 1000)
effective_bandwidth = calculate_effective_bandwidth(file_size_bits, rtt_ms)

print(f"Transmission Time: {transmission_time_s:.3f} seconds")
print(f"Round-Trip Time: {rtt_ms:.3f} milliseconds")
print(f"Effective Bandwidth: {effective_bandwidth / 1_000_000:.3f} Mbps")
