#!/usr/bin/gnuplot -persist

set xrange [1.e-2:8]
set logscale x 10
set logscale y 10
set term postscript
set key bottom left

set xlabel "y"
set ylabel "|u(t,x)-f(y)|"

load "plotter.gp"
