#!/bin/sh
ip addr flush to 0.0.0.0/0
ip route flush table all
ip addr add 192.168.2.1/24 dev eth0
ip route add default via 192.168.2.254
python httpd.py
tail -f /dev/null