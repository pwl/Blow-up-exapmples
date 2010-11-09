#!/usr/bin/gnuplot -persist

set xlabel "r"
set ylabel "u(t,r)"
set xrange [1.e-6:pi]
set yrange [0:pi]
set ytics (0, "{/Symbol p}/2" pi/2, "{/Symbol p}" pi, "{/Symbol p}/2+b_1" pi/2+0.573141133043885)
set grid ytics
set logscale x 10

# set xrange [ -2:2 ];
# set yrange [ -2:2 ];
# set trange [ 0:2*pi ];
# set size square;
set nokey
# set noxtic; set noytic;
# unset border;
# unset grid;

set object 1 rect from screen 0, 0, 0 to screen 1, 1, 0 behind
set object 1 rect fc  rgb "white"  fillstyle solid 1.0

# set logscale x 10
set term svg dynamic enhanced# size 800,600
set key off
# set ytics ("0" 0, "pi" pi, "2pi" 2*pi, "3pi" 3*pi, "4pi" 4*pi)
# set ytics ("0" 0, "pi" pi, "2pi" 2*pi)
set grid

load "plotter.gp"
