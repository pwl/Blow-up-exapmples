#!/usr/bin/gnuplot -persist

set xlabel "x"
set ylabel "u(x)"
set yrange [0:4*pi]
set xrange [0:pi]

# unset logscale
set term png
set key off
set ytics ("0" 0, "pi" pi, "2pi" 2*pi, "3pi" 3*pi, "4pi" 4*pi)
# set ytics ("0" 0, "pi" pi, "2pi" 2*pi)
set grid

load "plotter.gp"
