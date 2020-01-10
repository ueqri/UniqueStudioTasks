#!/bin/sh
apk update
apk add py-pip
pip install pyftpdlib -i https://pypi.tuna.tsinghua.edu.cn/simple

ip addr flush to 0.0.0.0/0
ip route flush table all
ip addr add 202.114.65.4/24 dev eth0
ip route add default via 202.114.65.1

python ftpd.py

tail -f /dev/null