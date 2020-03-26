#!/bin/bash
_instances=1

if ! [ -z "$1" ]; then
    _instances=$1
fi
cd bin
mpirun --hostfile ../host_file -N $_instances fractal_cluster