#!/bin/bash

dir=harvester_data

for file in $dir/shrinker*; do
    ./energy.sh $file
done
