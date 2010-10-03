#!/usr/bin/gnuplot -persist

x_min = 500
x_max = 1000
file = "log/info_1/logN100_rkf45_te-14.dat"
FIT_LIMIT = 1.e-20

load "plotter.gp"

a=0.01
T=0.3
A=2.

f(t) = T-A*exp(-2*a*t)

fit [x_min:x_max] f(x) file u 1:2 every 50 via T,A

# set logscale y 10

# plot file u 1:(abs($2-f($1))) w l

set print "T.dat" append
print x_min, " ", x_max, " ", T, " ", A
