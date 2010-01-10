#!/usr/bin/gnuplot -persist

set title "Higher blowup profiles for harmonic map flow"

set terminal postscript
set output "blowup.ps"

# set size square
set xrange [-11:5]
set yrange [1:2.5]
set xlabel "log(x)"
set ylabel "f(x)"
# set nokey

# set style data lines

load "bisection.gp"
