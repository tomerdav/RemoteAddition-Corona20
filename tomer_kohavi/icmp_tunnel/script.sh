modprobe tun
ip link set dev tun0 up
route add -net 0.0.0.0 netmask 0.0.0.0 metric 0 dev tun0
