#!/bin/bash

file=$1
dy=0.0001

blocks=$(./block.awk $file)

k=$(echo "$file" |sed -e 's|.*_k\([.0-9][.0-9]*\).*|\1|')
l=$(echo "$file" |sed -e 's|.*_l\([.0-9][.0-9]*\)\.dat|\1|')
l=50

for b in $(seq 1 $blocks ); do
    sum=$(nawk -v "block=$((b-1))" -v "N=3" -v "L=$l" -f scalar_prod.awk  $file \
	| spline -t 0.0 50.0 $dy -P15 2> /dev/null\
	| awk -f sum.awk )
    normyuy=$(nawk -v "block=$((b-1))" -v "N=3" -v "L=$l" -f scalar_norm_yuy.awk  $file \
	| spline -t 0.0 50.0 $dy -P15 2> /dev/null\
	| awk -f sum.awk )
    normus=$(nawk -v "block=$((b-1))" -v "N=3" -v "L=$l" -f scalar_norm_us.awk  $file \
	| spline -t 0.0 50.0 $dy -P15 2> /dev/null\
	| awk -f sum.awk )


    sum=$(echo "scale=20;$sum*$dy"|bc)
    normyuy=$(echo "scale=20;$normyuy*$dy"|bc)
    normus=$(echo "scale=20;$normus*$dy"|bc)

    E=$(echo "scale=20;$sum/sqrt($normyuy*$normus)"|bc -l)
    echo "$E"
done
