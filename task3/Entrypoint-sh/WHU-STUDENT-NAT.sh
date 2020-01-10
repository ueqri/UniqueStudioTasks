#!/bin/sh
ip addr flush to 0.0.0.0/0
ip route flush table all
ip addr add 202.114.66.254/24 dev eth0
ip addr add 192.168.30.254/24 dev eth1
ip route add default via 202.114.66.1

iptables -t nat -A POSTROUTING -s 192.168.30.0/24 -o eth0 -j SNAT --to-source 202.114.66.254
# iptables -t nat -A PREROUTING -s 202.114.66.254 -o eth1 -j SNAT --to-destination 192.168.30.0/24