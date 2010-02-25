#!/bin/bash

file=$1
dy=0.00001

blocks=$(./block.awk $file)

echo "$file"
k=$(echo "$file" |sed -e 's|.*_k\([.0-9][.0-9]*\).*|\1|')
l=$(echo "$file" |sed -e 's|.*_l\([.0-9][.0-9]*\)\.dat|\1|')

# echo "$k $l"

for b in $(seq 1 $blocks ); do
    sum=$(nawk -v "block=$((b-1))" -v "N=$k" -v "L=$l" -f energy.awk  $file \
	| spline -t 0.0 8.0 $dy -P15 2> /dev/null\
	| awk -f sum.awk )
    E=$(echo "$sum*$dy"|bc)
    echo "n=$b E=$E"
done
