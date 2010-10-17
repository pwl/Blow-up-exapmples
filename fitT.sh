#!/bin/bash

rm T.dat
incr=200

for i in $(seq $incr $incr 1500); do
    for j in $(seq 0 $incr $((i-incr))); do
	echo -ne "x_max = $i\nx_min = $j" > plotter.gp
	echo "i = $i, j = $j"
	./fitT.gp 2> /dev/null
    done
done
