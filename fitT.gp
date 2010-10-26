#!/usr/bin/gnuplot -persist

x_min = 1100
x_max = 12000
file = "log/info_1/log000.dat"
FIT_LIMIT = 1.e-20

load "plotter.gp"

a=0.005
T=0.3
A=2.

f(t) = T-A*exp(-a*t)

fit [x_min:x_max] f(x) file u 1:2 every 10 via T,A,a

# set logscale y 10

# plot file u 1:(abs($2-f($1))) w l

set print "T.dat" append
print x_min, " ", x_max, " ", T, " ", A, " ", a
