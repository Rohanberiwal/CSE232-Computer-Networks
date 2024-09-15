# README: VPN IP Address Allocation

## Overview

This document explains how Virtual Private Networks (VPNs) manage IP addresses for devices that connect through them. It covers how VPNs assign virtual IP addresses and how these relate to the device's original local and public IP addresses.

## VPN Overview

**Virtual Private Network (VPN):**
- **Purpose:** Provides a secure and encrypted connection over a public network (like the Internet) to a private network (such as a corporate LAN).
- **Function:** Creates a secure tunnel for data transmission, allowing devices to access resources on the private network as if they were directly connected.

## IP Address Allocation in VPNs

### 1. Virtual IP Address Assignment

- **Virtual IP Address:**
  - **Assignment:** When a device connects to a VPN, it is assigned a new virtual IP address by the VPN server.
  - **Purpose:** This virtual IP address is used for communication within the VPN tunnel, allowing the VPN to manage network access and routing.

- **Device’s Original IP Address:**
  - **Local Network IP:** The device retains its original IP address assigned by its local network (e.g., home or office network).
  - **Usage:** This IP address is used for local network communication and is separate from the VPN’s virtual IP address.

### 2. IP Address Visibility

- **Public IP Address:**
  - **Through the VPN:** External resources (such as websites) see the public IP address of the VPN server, not the device’s original public IP address.
  - **Reason:** The VPN server acts as an intermediary for the device’s internet traffic, masking the device’s original public IP address.

- **Private IP Address:**
  - **Within the VPN Network:** Devices use their assigned virtual IP addresses for communication within the VPN network. This allows access to resources on the private network securely.

### 3. Scenarios Where VPN Changes IP Address

- **Connecting to a VPN Server:**
  - **New IP Assignment:** Upon connecting, the device receives a new virtual IP address from the VPN server for use within the VPN network.

- **Accessing Remote Resources:**
  - **Private Network Access:** The virtual IP address enables the device to access resources on the private network as if it were physically present on that network.

### 4. Scenarios Where VPN Does Not Change IP Address

- **Local Network Communication:**
  - **Local IP Address:** The device’s IP address on its local network remains unchanged. The VPN does not alter the device’s IP address for local network communication.

## Summary

- **VPN Connection:** Devices receive a new virtual IP address from the VPN server when connecting, used within the VPN network.
- **Original IP Address:** The device retains its original local IP address for local network communication.
- **Public IP Address:** External resources see the public IP address of the VPN server, not the device’s original public IP address.

By understanding how VPNs handle IP addresses, you can better manage network configurations and security when using VPNs for secure remote access.

