#!/usr/bin/gnuplot -persist

set xlabel "x"
set ylabel "u(x)"
set xrange [1.e-8:pi]
set yrange [0:pi]

set logscale x 10
set term png
set key off
# set ytics ("0" 0, "pi" pi, "2pi" 2*pi, "3pi" 3*pi, "4pi" 4*pi)
# set ytics ("0" 0, "pi" pi, "2pi" 2*pi)
set grid

load "plotter.gp"
