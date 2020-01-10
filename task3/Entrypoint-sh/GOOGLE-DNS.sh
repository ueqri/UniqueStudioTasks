#!/bin/sh
ip addr flush to 0.0.0.0/0
ip route flush table all 
ip addr add 8.8.8.8/24 dev eth0
ip route add default via 8.8.8.1

echo "8.8.4.4  www.google.com" >> /etc/hosts
echo "182.61.200.6  www.baidu.com" >> /etc/hosts
echo "202.200.79.2  www.gov.cn" >> /etc/hosts

cp /etc/dnsmasq.conf /etc/dnsmasq.conf_bak
rm -f /etc/dnsmasq.conf
cat >> /etc/dnsmasq.conf <<EOF
expand-hosts
local=/google.com/
domain-needed

no-resolv
cache-size=500
 
log-queries
log-facility=/var/log/dnsmasq.log
log-async=20

address=/doubleclick.net/127.0.0.1
address=/.phobos.apple.com/202.175.5.114
EOF
dnsmasq

tail -f /dev/null