#!/bin/bash

f=$1
a=$2
file_interpolated="splined.dat"
spline="spline -t 0.0 8.0 .001 -P 15"

cat $f | grep -v '#'  | awk "{print \$1*$a, \$2}"  | $spline  | join \
      - $file_interpolated | awk "{print \$1,\$2-\$3}"
