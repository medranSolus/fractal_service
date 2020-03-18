#!/bin/bash
_instances=1

if ! [ -z "$1" ]; then
    _instances=$1
    proc_number=$(nproc)
    if (( _instances > proc_number )); then
        _instances=$proc_number
        echo WARNING: not enough logical processor to start $1 instances!
    fi
fi

mpirun -N $_instances bin/frac_cluster