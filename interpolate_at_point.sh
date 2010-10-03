#!/bin/bash

# Interpolate data from $1 at points given by the first column of $2

dim=$(head $1 -n1|wc -w)
dim=$((dim-1))

for point in $(awk '/^[0-9]/ {print $1}' $2); do # this selects the first line
    val=$(spline -d$dim -s -P25 -t $point $point -n1 $1 2> /dev/null |head -n1)
    if [ "$val" != "" ]; then
	echo $point $val
    fi
done
