#!/usr/bin/gnuplot -persist

set term epslatex # size 3,3
set output "graphics/Shrinker_profiles.tex"
# set key bottom left

set size 1.2,1.2

set xrange [-15:8]
set yrange [0:pi]

set xlabel '$\ln(y)$'
set ylabel '$f_n(y)$' offset graph .05

set ytics 0,pi/4,pi format ''
set ytics add ('0' 0, '$\pi/2$' pi/2,  '$\pi$' pi)

unset key
unset title

plot "./harvester_data/shrinker_k3.00000_l1.0.dat" w l, pi/2


