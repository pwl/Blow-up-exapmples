#!/usr/bin/gnuplot -persist

set xlabel "log(tan(x/2))"
set ylabel "u(x) & slow time"
set yrange [0:]
set xrange [0:pi]
set xrange [-15:15]

# unset logscale
# set term png
set key off
set ytics pi format ""
set ytics add ("0" 0, "pi" pi, "2pi" 2*pi, "3pi" 3*pi, "4pi" 4*pi)
# set ytics 2*pi
# set ytics ("0" 0, "pi" pi, "2pi" 2*pi, "3pi" 3*pi)
set grid

set title "pattern for harmonic maps"

load "plotter.gp"
