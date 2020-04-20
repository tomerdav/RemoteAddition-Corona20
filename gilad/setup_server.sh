#!/bin/bash

ifconfig tun1 up
echo 1 > /proc/sys/net/ipv4/ip_forward
iptables -t nat -A PREROUTING -p icmp -j DNAT --to-destination 49.49.49.49
ip route add 49.49.49.49 dev tun1
ip route flush cache