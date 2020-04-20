#!/bin/bash

ifconfig tun0 up
ip rule add fwmark 2 table 3
ip route add default dev tun0 table 3
ip route flush cache
iptables -t mangle -A OUTPUT ! -p icmp -j MARK --set-mark 2
