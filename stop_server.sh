#!/bin/bash
# Run as root

systemctl start nftables
systemctl stop nfs-server
systemctl stop sshd