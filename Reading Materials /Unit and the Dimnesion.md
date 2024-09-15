# Networking Units and Conversions

## Overview

This README provides information on common networking units used for data measurement, along with formulas for converting between these units. Understanding these units and conversions is essential for evaluating network performance and data transfer.

## Common Networking Units

1. **Bits (b)**
   - **Definition:** The smallest unit of data in networking.
   - **Unit Symbol:** `b` (lowercase)

2. **Bytes (B)**
   - **Definition:** A group of 8 bits.
   - **Unit Symbol:** `B` (uppercase)
   - **Conversion:** 
     \[
     1 \text{ Byte (B)} = 8 \text{ Bits (b)}
     \]

3. **Kilobits (Kb)**
   - **Definition:** 1,000 bits.
   - **Unit Symbol:** `Kb` (lowercase)
   - **Conversion:**
     \[
     1 \text{ Kilobit (Kb)} = 1{,}000 \text{ Bits (b)}
     \]

4. **Kilobytes (KB)**
   - **Definition:** 1,024 bytes.
   - **Unit Symbol:** `KB` (uppercase)
   - **Conversion:**
     \[
     1 \text{ Kilobyte (KB)} = 1{,}024 \text{ Bytes (B)} = 8{,}192 \text{ Bits (b)}
     \]

5. **Megabits (Mb)**
   - **Definition:** 1,000,000 bits.
   - **Unit Symbol:** `Mb` (lowercase)
   - **Conversion:**
     \[
     1 \text{ Megabit (Mb)} = 1{,}000 \text{ Kilobits (Kb)} = 1{,}000{,}000 \text{ Bits (b)}
     \]

6. **Megabytes (MB)**
   - **Definition:** 1,024 megabytes.
   - **Unit Symbol:** `MB` (uppercase)
   - **Conversion:**
     \[
     1 \text{ Megabyte (MB)} = 1{,}024 \text{ Kilobytes (KB)} = 8{,}388{,}608 \text{ Bits (b)}
     \]

7. **Gigabits (Gb)**
   - **Definition:** 1,000,000,000 bits.
   - **Unit Symbol:** `Gb` (lowercase)
   - **Conversion:**
     \[
     1 \text{ Gigabit (Gb)} = 1{,}000 \text{ Megabits (Mb)} = 1{,}000{,}000{,}000 \text{ Bits (b)}
     \]

8. **Gigabytes (GB)**
   - **Definition:** 1,024 megabytes.
   - **Unit Symbol:** `GB` (uppercase)
   - **Conversion:**
     \[
     1 \text{ Gigabyte (GB)} = 1{,}024 \text{ Megabytes (MB)} = 8{,}589{,}869{,}056 \text{ Bits (b)}
     \]

## Conversion Formulas

1. **Bits to Bytes:**
   \[
   \text{Bytes} = \frac{\text{Bits}}{8}
   \]

2. **Bytes to Bits:**
   \[
   \text{Bits} = \text{Bytes} \times 8
   \]

3. **Kilobits to Bits:**
   \[
   \text{Bits} = \text{Kilobits} \times 1{,}000
   \]

4. **Bits to Kilobits:**
   \[
   \text{Kilobits} = \frac{\text{Bits}}{1{,}000}
   \]

5. **Megabits to Bits:**
   \[
   \text{Bits} = \text{Megabits} \times 1{,}000{,}000
   \]

6. **Bits to Megabits:**
   \[
   \text{Megabits} = \frac{\text{Bits}}{1{,}000{,}000}
   \]

7. **Gigabits to Bits:**
   \[
   \text{Bits} = \text{Gigabits} \times 1{,}000{,}000{,}000
   \]

8. **Bits to Gigabits:**
   \[
   \text{Gigabits} = \frac{\text{Bits}}{1{,}000{,}000{,}000}
   \]

## Python Code for Unit Conversion

```python
# Conversion functions

def bits_to_bytes(bits):
    return bits / 8

def bytes_to_bits(bytes):
    return bytes * 8

def kilobits_to_bits(kilobits):
    return kilobits * 1_000

def bits_to_kilobits(bits):
    return bits / 1_000

def megabits_to_bits(megabits):
    return megabits * 1_000_000

def bits_to_megabits(bits):
    return bits / 1_000_000

def gigabits_to_bits(gigabits):
    return gigabits * 1_000_000_000

def bits_to_gigabits(bits):
    return bits / 1_000_000_000

# Example usage
file_size_kb = 125
file_size_bits = file_size_kb * 1024 * 8
bandwidth_mbps = 1
bandwidth_bps = bandwidth_mbps * 1_000_000
transfer_time = file_size_bits / bandwidth_bps

print(f"File size in bits: {file_size_bits}")
print(f"Bandwidth in bps: {bandwidth_bps}")
print(f"Time to transfer file: {transfer_time} seconds")
