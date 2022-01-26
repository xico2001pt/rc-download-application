#!/bin/bash

ip addr flush eth0
ip addr add 172.16.51.1/24 dev eth0
ip route add 172.16.50.0/24 via 172.16.51.253
ip route add default via 172.16.51.254