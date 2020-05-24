#!/bin/bash


if [ -z "$1" ]; then
    echo "Specify output size!"
    exit 1
fi

rm -rf bin/jobs/
./run.sh "$2" "$3" "$4" "$5"&
cd bin && ./cluster_test "$1"