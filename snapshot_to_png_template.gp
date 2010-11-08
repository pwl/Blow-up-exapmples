#!/usr/bin/gnuplot -persist

# set xlabel "x"
# set ylabel "u(x)"
# set xrange [1.e-8:pi]
# set yrange [0:pi]
set parametric;
set xrange [ -2:2 ];
set yrange [ -2:2 ];
set trange [ 0:2*pi ];
set size square;
set nokey
set noxtic; set noytic;
unset border;
unset grid;

# set logscale x 10
set term svg dynamic# size 800,600
set key off
# set ytics ("0" 0, "pi" pi, "2pi" 2*pi, "3pi" 3*pi, "4pi" 4*pi)
# set ytics ("0" 0, "pi" pi, "2pi" 2*pi)
set grid

load "plotter.gp"
