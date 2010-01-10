#!/usr/bin/gnuplot -persist

set key right top
set key off

set xlabel "y"
set ylabel "H(y)"

set xrange [0:10]

set title "Self similar profile for harmonic map flow"
# set term png giant size 800, 600
# set output "self_sim.png"

load "plotter.gp"
