#!/usr/bin/gnuplot -persist

expander_file="./harvester_data/expander_k3.00000_l1.0.dat"
shrinker_file="./harvester_data/shrinker_k3.00000_l1.0.dat"

b1=0.573141

set terminal postscript eps color enhanced font 20 solid
set output "graphics/glue.eps"

set ytics (0, '{/Symbol p}/2' pi/2,  '{/Symbol p}' pi, 'b_1+{/Symbol p}/2' b1+pi/2)
set grid

set xrange [:10]
set yrange [0:pi]
set xlabel 'y'

set key spacing 5.

plot expander_file u 1:(pi/2-$2) w l lw 3 t 'Expander ({/Symbol p} - F_A_1)',\
     shrinker_file index 0 w l lw 3 t 'Shrinker (f_1)',\
     b1+pi/2 lw 3 t ''
