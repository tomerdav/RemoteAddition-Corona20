#!/bin/bash


iptables -t mangle -D OUTPUT ! -p icmp -j MARK --set-mark 2
ip route del default dev tun0 table 3
ip rule del fwmark 2 table 3
ip route flush cache
