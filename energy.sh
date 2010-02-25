#!/bin/bash

file=$1
dy=0.00001

blocks=$(./block.awk $file)

echo "$file"

for b in $(seq 1 $blocks ); do
    sum=$(nawk -f energy.awk "block=$((b-1))" $file \
	| spline -t 0.0 8.0 $dy -P15 2> /dev/null\
	| awk -f sum.awk )
    E=$(echo "$sum*$dy"|bc)
    echo "n=$b E=$E"
done
