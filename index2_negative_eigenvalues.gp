#!/usr/bin/gnuplot -persist
set title "Negative eigenvalues for index 2 solution"
set terminal postscript
set output "index2_negative_eigenvalues.ps"
set grid

plot [] [:1] "./eigenfile_index2.dat" u 1:(-$5) every ::2 ps 1 pt 2 t ""
