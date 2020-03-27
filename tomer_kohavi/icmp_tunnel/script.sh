modprobe tun
ip link set dev tun0 up
route add -net 192.168.0.0/24 metric 0 dev tun0

