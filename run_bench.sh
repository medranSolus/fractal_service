#!/bin/bash

rm -rf bin/jobs/
./run.sh "$1" "$2"&
cd bin && ./cluster_test