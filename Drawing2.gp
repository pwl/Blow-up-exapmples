#!/usr/bin/gnuplot -persist

# dim=2.1


# omega=sqrt(8.*dim-8.-dim*dim)/2.
# bratio=-(dim-2.)*pi/omega/2.

# set output "graphics/roots.eps"

set format x '10^{%L}'
set ytics (0, '{/Symbol p}/2' pi/2,  '{/Symbol p}' pi)

set xlabel "A"
set ylabel "B"

set logscale x
unset key

set xrange [:1.e5]
set yrange [0:pi]

# plot file w l lt 1 lw 2, "" u 1:(pi-$2) w l lt 3 lw 2, for [d in blist] pi/2.+d lt 1

load "plotter.gp"
