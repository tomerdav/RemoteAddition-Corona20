#!/bin/bash

# turn UP tun1
ip link set tun1 up

# delete exist default gateway
route del default

# add defualt gateway through tun1
route add default metric 0 dev tun1 
