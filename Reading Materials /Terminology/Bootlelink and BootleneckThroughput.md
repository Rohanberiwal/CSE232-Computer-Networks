# README: Bottleneck Link and Bottleneck Throughput

## Overview

In network performance, the terms **Bottleneck Link** and **Bottleneck Throughput** are used to describe limitations that affect the overall speed and efficiency of a network. Understanding these concepts helps in diagnosing and improving network performance issues.

## Bottleneck Link

**Definition:**
- **Bottleneck Link** refers to the segment of a network that has the lowest bandwidth or highest latency, which limits the performance of the entire network.

**Characteristics:**
- **Limitations:** This link is the slowest part of the network, creating a constraint on data flow and overall network speed.
- **Impact:** The performance of the entire network is affected by this link. Even if other parts of the network have higher capacity, the bottleneck link restricts the maximum achievable speed.

**Example:**
- Imagine a network with links of 10 Mbps, 100 Mbps, and 1 Gbps speeds. If the slowest link is 10 Mbps, this link is the bottleneck. All data traffic is limited by the 10 Mbps speed.

## Bottleneck Throughput

**Definition:**
- **Bottleneck Throughput** is the maximum data transfer rate constrained by the bottleneck link. It represents the actual throughput achieved due to the limitations of the slowest link.

**Characteristics:**
- **Measurement:** It is the effective data transfer rate observed when the network's performance is limited by the bottleneck link.
- **Performance:** The throughput is reduced to the capacity of the bottleneck link, even if other parts of the network can handle higher speeds.

**Example:**
- In a network where the bottleneck link is 10 Mbps, the maximum throughput of the network will also be limited to 10 Mbps, regardless of the higher speeds available on other links.

## Summary

- **Bottleneck Link:** The network segment with the lowest bandwidth or highest latency that limits overall network performance.
- **Bottleneck Throughput:** The effective data transfer rate constrained by the bottleneck link, reflecting the network's maximum achievable speed based on the slowest link.

**Understanding these concepts** helps in identifying performance issues and making improvements to ensure that no single component of the network is limiting overall efficiency.

