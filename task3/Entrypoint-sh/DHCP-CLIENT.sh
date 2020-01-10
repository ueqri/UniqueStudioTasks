#!/bin/sh
apk add dhcpcd
ip addr flush to 0.0.0.0/0
ip route flush table all
dhcpcd
tail -f /dev/null