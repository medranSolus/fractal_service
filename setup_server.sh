#!/bin/bash
# Run as root

# Do this only once!
if [ -z "$1" ]; then
    if [ "$1" == "first" ]; then
        echo "/home/mpi_fractal/fractal_cluster *(rw,async,no_root_squash,no_subtree_check)" >> /etc/exports
    fi
    echo "Unknown command!"
    exit -1
fi

# NFS server must be started and firewall disabled
systemctl stop nftables
systemctl start nfs-server
systemctl start sshd
exportfs -a