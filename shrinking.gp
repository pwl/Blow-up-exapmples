#!/usr/bin/gnuplot -persist

s="./harvester_data_shrinker/shrinker_reversed_k3.00000_l1.0.dat"

set key off
set xlabel "r"
set ylabel "u(r,t)"
set xrange [0:10]
set yrange [0:pi]
set terminal postscript enhanced color solid font 25
# set terminal postscript enhanced color
set output "graphics/shrinking.eps"
set ytics (0, "{/Symbol p}/2" pi/2, "{/Symbol p}" pi)
set grid

plot  s w l lt 1 lw 2, s u ($1/2.):2 w l lt 1 lw 2, s u ($1/4.):2 w l lt 1 lw 2, s u ($1/8.):2 w l lt 1 lw 2, s u ($1/16.):2 w l lt 1 lw 2


set output "graphics/const.eps"

plot pi/2+0.573141133043885 lt 1 lw 3
