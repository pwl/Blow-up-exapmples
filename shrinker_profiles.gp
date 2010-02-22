#!/usr/bin/gnuplot -persist

set terminal postscript
set logscale x
set xrange [1.e-3:]
set xlabel "y"
set ylabel "f(y)"

load "plotter.gp"
