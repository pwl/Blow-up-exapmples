#!/usr/bin/gnuplot -persist
set title "Positive eigenvalues for index 2 solution"
set terminal postscript
set output "index2_positive_eigenvalues.ps"
set grid

plot "./eigenfile_index2.dat" u 1:(-$5) every ::::0 ps 1 pt 2 t ""
