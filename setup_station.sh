#!/bin/bash
# Every machine should have this script
# Run as root

# Set IP according to master machine
systemctl stop nftables
systemctl start nfs-server
systemctl start sshd
mount -t nfs 192.168.0.108:/home/mpi_fractal/fractal_cluster /home/mpi_fractal/fractal_cluster