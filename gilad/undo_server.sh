#!/bin/bash

iptables -t nat -D PREROUTING -p icmp -j DNAT --to-destination 49.49.49.49
ip route del 49.49.49.49 dev tun1
ip route flush cache