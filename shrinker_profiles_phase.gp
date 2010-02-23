#!/usr/bin/gnuplot -persist

set terminal postscript
set xrange [-pi/2-.1:pi/2+.1]
# set yrange [-10:10]
set xlabel "f"
set ylabel "f\'"
set xtics (0)
set ytics (0)
set grid

load "plotter.gp"
