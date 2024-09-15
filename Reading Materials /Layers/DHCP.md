# README: DHCP IP Address Allocation and Configuration

## Overview

This document explains how the Dynamic Host Configuration Protocol (DHCP) handles IP address allocation, including scenarios when devices move within or across network segments.

## DHCP Overview

**Dynamic Host Configuration Protocol (DHCP):**
- **Purpose:** Automatically assigns IP addresses and other network configuration parameters (e.g., subnet mask, gateway, DNS) to devices on a network.
- **Process:**
  - **DHCP Discover:** Device broadcasts a request to find a DHCP server.
  - **DHCP Offer:** DHCP server offers an IP address to the device.
  - **DHCP Request:** Device requests the offered IP address.
  - **DHCP Acknowledgment:** DHCP server confirms the assignment and provides configuration details.

## IP Address Allocation

**1. Intra-LAN Movement:**
- **Same Subnet:** When a device moves within the same LAN or subnet (e.g., from one switch port to another), the DHCP server typically does not reallocate a new IP address.
- **Lease Renewal:** The device continues to use the same IP address as long as it maintains its DHCP lease. The lease is renewed periodically to ensure the address remains valid.

**2. Cross-Subnet Movement:**
- **Different Subnet:** If a device moves to a different subnet, it must obtain a new IP address appropriate for the new subnet.
  - **DHCP Request:** The device will broadcast a new DHCP Discover message to locate a DHCP server in the new subnet.
  - **New IP Assignment:** The DHCP server in the new subnet assigns a new IP address from its pool, and the device updates its network configuration accordingly.

## DHCP Lease Management

**1. Lease Expiry:**
- **Expiry:** If a device’s DHCP lease expires, it must request a new IP address or renew its current lease if it is still valid.
- **Reconnection:** Upon reconnection, the device may request a new IP address if the previous lease has expired or is no longer valid.

**2. Manual Release:**
- **Release:** If a device manually releases its IP address, it will request a new address from the DHCP server upon reconnection.
- **Address Reuse:** The DHCP server may reassign the released IP address to another device if it is within the address pool.

## Network Reconfiguration

**1. DHCP Scope Changes:**
- **Scope Changes:** If the DHCP scope (range of IP addresses) is reconfigured or extended, devices may need to obtain new IP addresses within the new scope.
- **Policy Updates:** Network policies or address management strategies may necessitate new IP allocations.

**2. Configuration Adjustments:**
- **DHCP Settings:** Adjustments to DHCP settings (e.g., changing subnet masks or gateways) may require devices to renew their IP leases or request new addresses.

## Summary

- **Intra-LAN Movement:** Devices moving within the same subnet retain their IP address and lease as managed by the DHCP server.
- **Cross-Subnet Movement:** Devices moving to a different subnet will request a new IP address from a DHCP server in the new subnet.
- **Lease Management:** DHCP handles IP address assignment, lease renewal, and updates based on network configuration and policies.

By understanding DHCP's role in IP address allocation and lease management, you can effectively manage network configurations and ensure devices receive appropriate IP addresses for their network environment.

