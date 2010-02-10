#!/usr/bin/gnuplot -persist

set terminal postscript
set output "eigenvalue_plot.ps"

set title "Largest eigenvalues for shrinkers of index 2\nfor L smallest (1.0, top) to largest (3.0, bottom)"
set xlabel "Dimension"
set ylabel "Eigenvalue"


load "plotter.gp"
