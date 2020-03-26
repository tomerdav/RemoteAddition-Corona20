#!/bin/bash

# Set up ip a
sudo ip addr add 10.0.2.69/24 dev tun0
sudo ip addr add 10.0.0.69/16 dev tun0

# Iface state
sudo ip link set tun0 up

# Set up routes with metric
route add -net 10.0.2.0/24 metric 1 dev tun0
route add -net 10.0.0.0/16 metric 1 dev tun0
