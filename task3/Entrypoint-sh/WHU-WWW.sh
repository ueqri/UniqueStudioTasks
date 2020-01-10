#!/bin/sh
ip addr flush to 0.0.0.0/0
ip route flush table all
ip addr add 202.114.65.2/24 dev eth0
ip route add default via 202.114.65.1
python httpd.py
tail -f /dev/null