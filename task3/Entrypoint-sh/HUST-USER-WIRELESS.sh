#!/bin/sh
ip addr flush to 0.0.0.0/0
ip route flush table all

ip addr add 202.114.12.4/24 dev eth0
ip addr add 10.1.1.254/24 dev eth1
ip route add default via 202.114.12.1

cp /etc/dnsmasq.conf /etc/dnsmasq.conf_bak
rm -f /etc/dnsmasq.conf
cat >> /etc/dnsmasq.conf <<EOF
listen-address=10.1.1.254
interface=eth1
bind-interfaces
dhcp-range=10.1.1.10,10.1.1.123,12h
dhcp-option=6,8.8.8.8,8.8.4.4
port=5353
EOF
dnsmasq

iptables -t nat -A POSTROUTING -s 10.1.1.0/24 -o eth0 -j SNAT --to-source 202.114.12.4
# iptables -t nat -A PREROUTING -s 202.114.66.254 -o eth1 -j SNAT --to-destination 192.168.30.0/24