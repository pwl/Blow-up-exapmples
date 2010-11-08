#!/usr/bin/gnuplot

B="test.dat"

set terminal postscript enhanced color solid font 25
set output "graphics/b1_B_match.eps"

set logscale x

set xlabel "A"
set yrange [0:pi]
set ytics (0, "{/Symbol p}/2" pi/2, "{/Symbol p}" pi)

set grid

plot B w l t "B(A)" lw 3, pi/2.+0.5741 t "b_1+{/Symbol p}/2" lw 3

set output "graphics/pi_b1_B_match.eps"

plot B u 1:(pi-$2) w l t "{/Symbol p}-B(A)" lw 3, pi/2.+0.5741 t "b_1+{/Symbol p}/2" lw 3
