#!/usr/bin/gnuplot -persist

set format x '10^{%L}'
# set format y '10^{%L}'

set xlabel "A"
set ylabel "B"

set logscale x 10
unset key

set xrange [:1.e8]
set yrange [-.6:.6]
# set yrange [1.e-14:]


load "plotter.gp"
