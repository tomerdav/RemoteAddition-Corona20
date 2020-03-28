#!/bin/bash


ifconfig tun0 up
#route add -net 0.0.0.0 netmask 0.0.0.0 metric 0 dev tun0
ip rule add fwmark 2 table 3
ip route add default dev tun0 table 3
ip route flush cache
iptables -t mangle -A OUTPUT ! -p icmp -j MARK --set-mark 2
