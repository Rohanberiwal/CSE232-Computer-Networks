# README: ARP and RARP in Packet Fragmentation

## Overview

This document explains how the Address Resolution Protocol (ARP) and Reverse Address Resolution Protocol (RARP) work in routers and their roles in the process of packet fragmentation and forwarding.

## Address Resolution Protocol (ARP)

### Purpose
- **ARP** is used to map an IP address to a MAC (Media Access Control) address, facilitating communication on the data link layer (Layer 2) of a network.

### ARP Process

1. **ARP Request:**
   - **Initiation:** When a device needs to send a packet to another device within the same local network, it needs the destination device's MAC address.
   - **Broadcast:** The device sends an ARP request packet to all devices on the local network asking for the MAC address associated with a specific IP address.

2. **ARP Reply:**
   - **Response:** The device with the matching IP address responds with an ARP reply packet containing its MAC address.
   - **Update:** The requesting device updates its ARP cache with this IP-to-MAC mapping for future use.

### Usage in Fragmentation
- **Fragment Transmission:** Before forwarding fragmented packets, a router uses ARP to resolve the MAC address of the next hop. Each fragment is encapsulated in a frame with the resolved MAC address.

## Reverse Address Resolution Protocol (RARP)

### Purpose
- **RARP** maps a MAC address to an IP address. It has been largely replaced by DHCP but was historically used for devices to obtain their IP addresses.

### RARP Process

1. **RARP Request:**
   - **Initiation:** When a device only knows its MAC address and needs to find its IP address, it sends out a RARP request.
   - **Broadcast:** The request is broadcast to all RARP servers on the local network.

2. **RARP Reply:**
   - **Response:** A RARP server responds with the IP address associated with the requesting MAC address.
   - **Configuration:** The requesting device uses the provided IP address for further communication.

### Usage in Fragmentation
- **Legacy Use:** RARP was used to configure devices initially with an IP address. After configuration, ARP is used for subsequent communications, including handling fragmented packets.

## Fragmentation and Reassembly

### Fragmentation
- **Process:** When a packet is too large for the network's MTU (Maximum Transmission Unit), it is broken down into smaller fragments.
- **Reassembly:** Fragments are reassembled into the original packet at the destination before being passed to the transport layer.

### ARP's Role
- **Fragment Transmission:** ARP is used to resolve the MAC address of the next hop router. Each fragment is then sent with this MAC address in its data link layer frame.

### RARP's Role
- **Initial Configuration:** RARP was used for devices to determine their IP address before communication began. Fragmentation and forwarding are handled using ARP once the device has an IP address.

## Summary

- **ARP:** Resolves IP addresses to MAC addresses, essential for forwarding packets (including fragments) at the data link layer.
- **RARP:** Historically used for mapping MAC addresses to IP addresses, now largely replaced by DHCP.
- **Fragmentation:** Involves dividing large packets into smaller fragments, with ARP used to resolve MAC addresses for forwarding.

By understanding ARP and RARP, and their role in packet fragmentation, network devices can efficiently handle data transmission and ensure reliable communication.

