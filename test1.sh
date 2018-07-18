#!/bin/bash
echo "bash1 is running"
date
runtime=${1:-1442m}
mypid=$$
tshark -i enp3s0 -f 'udp port 2500 or udp port 2700' -w ~/wireshark/neda/loga_Jun_18.pcapng &
cpid=$!
sleep $runtime
kill -s SIGTERM $cpid
echo "Both processes are terminated".
date
