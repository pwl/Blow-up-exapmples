#!/usr/bin/gnuplot -persist

# set xrange [1.e-2:8]
# set logscale x 10
# set logscale y 10
set term latex # size 3,3
set output "graphics/Harmonic_static_to_multiplot.tex"
# set key bottom left

# set xlabel "y"
# set ylabel "|u(t,x)-f(y)|"

# set multiplot layout 3,3 scale 1.18,1.25 title "Convergence to blow-up profile in (s,y)"

load "plotter.gp"

# unset multiplot
