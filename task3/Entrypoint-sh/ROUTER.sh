#!/bin/sh
ip addr flush to 0.0.0.0/0
ip route flush table all
sed -i s'/ospfd=no/ospfd=yes/' /etc/quagga/daemons
service quagga start