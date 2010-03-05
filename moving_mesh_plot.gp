#!/usr/bin/gnuplot -persist

set title "Mesh movement"
set xlabel "Mesh points"
set ylabel "Time"
set logscale x 10

set terminal postscript
set output "moving_mesh_plot.ps"

load "plotter.gp"
