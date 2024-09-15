# README: BOOTP (Bootstrap Protocol)

## Overview

**BOOTP** (Bootstrap Protocol) is a network protocol used to automatically assign IP addresses and provide other configuration information to network devices, especially during the initial boot process. BOOTP was developed as a means to enable devices to receive necessary network configuration information without manual setup.

## Purpose

- **Automatic IP Address Assignment:** BOOTP helps devices obtain an IP address automatically when they join a network.
- **Configuration Information:** BOOTP provides additional network configuration details such as the default gateway, subnet mask, and the location of the boot file needed for the device to start up.

## How It Works

1. **Booting Process:**
   - When a device starts up and needs an IP address, it sends a BOOTP request message over the network.
2. **BOOTP Server Response:**
   - The BOOTP server receives the request and responds with the IP address and other configuration details.
   - The device uses this information to configure its network settings and continue the boot process.

## Key Features

- **Static Configuration:** BOOTP typically uses static mappings between MAC addresses and IP addresses. Each device is assigned a specific IP address based on its hardware address.
- **Simple Protocol:** BOOTP provides basic network configuration but lacks the dynamic features and flexibility of DHCP (Dynamic Host Configuration Protocol).

## Differences from DHCP

- **Dynamic Allocation:** Unlike DHCP, which dynamically allocates IP addresses from a pool and supports lease renewal, BOOTP uses fixed configurations.
- **Flexibility:** DHCP offers additional features like dynamic IP address allocation, lease management, and more comprehensive network configuration options compared to BOOTP.

## Use Cases

- **Network Booting:** BOOTP is used for booting devices over the network, such as diskless workstations or network appliances.
- **Legacy Systems:** It is also used in some older systems or environments that do not support DHCP.

## Summary

- **BOOTP (Bootstrap Protocol):** A protocol used to automatically assign IP addresses and provide network configuration details to devices during the boot process.
- **Function:** Devices send a BOOTP request to a BOOTP server, which responds with the necessary IP address and configuration.
- **Predecessor to DHCP:** BOOTP is simpler and less flexible than DHCP, which is now the standard for dynamic IP address management and network configuration.

By understanding BOOTP, you can appreciate its role in the history of network protocols and its fundamental purpose in providing network configuration during device startup.

