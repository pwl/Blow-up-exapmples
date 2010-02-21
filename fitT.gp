#!/usr/bin/gnuplot -persist

x_min = 900
x_max = 1000
file = "log/info_1/log000.dat"
FIT_LIMIT = 1.e-20

B=0.01
T=0.06
A=0.1

f(t) = T-A*exp(-B*t)

fit [x_min:x_max] f(x) file u 1:2 via T,A,B

set logscale y 10

plot file u 1:(abs($2-f($1))) w l

set print "T.dat"
print T
