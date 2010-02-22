#!/usr/bin/gnuplot -persist

set key right top
set key off

set xlabel "y"
set ylabel "H(y)"

set xrange [0:10]

set title "Self similar profile for harmonic map flow"
# set term png giant size 800, 600
# set output "self_sim.png"

gamma = .5+(sqrt(2)-1)/(2*(3-sqrt(2)))
lambda(t) = t**gamma
lambda(t) = 1

load "plotter.gp"
