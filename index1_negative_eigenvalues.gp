#!/usr/bin/gnuplot -persist
set title "Negative eigenvalues for index 1 solution"
set terminal postscript
set output "index1_negative_eigenvalues.ps"
set grid

plot [] [:1]  "./eigenfile_index1.dat" u 1:(-$5) ps 1 pt 2 t ""
