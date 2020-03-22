#!/bin/bash
_instances=1

#if ! [ -z "$1" ]; then
    #_instances=$1
#fi

mpirun --hostfile host_file -N $_instances bin/frac_cluster $1 $2 $3 $4 $5