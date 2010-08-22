#!/usr/bin/gnuplot -persist

d=3
Vn(y,f)=(d-1)*(d-3+4*cos(2*f))/4/y**2+(y**2-4*d)/16
Vnreg(y,f)=y**2*Vn(y,f)
file="./harvester_data/shrinker_k3.00000_l1.0.dat"
indices="0 1 3 6"

set terminal postscript enhanced color solid font 25
set output "graphics/Vinfty.eps"

set xrange [1.e-8:10]
set yrange [:2.5]

set xlabel "y"
set ylabel "y^2 V_n(y)"

set format x '10^{%L}'

set logscale x 10
set key left bottom

tit(istr)=sprintf("n = %s", istr)

plot for [i in indices] file u 1:(Vnreg($1,$2)) index int(i) w l lw 3 t tit(i), Vnreg(x,pi/2) lt -1 lw 2 t tit("{/Symbol \245}")
