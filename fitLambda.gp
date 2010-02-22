#!/usr/bin/gnuplot -persist

x_min = 600
x_max = 1000
file = "log/info_1/log000.dat"
FIT_LIMIT = 1.e-20

B=-1.5
T=0.33
A=2.
a=0.01
L1=0.51762191809152735544	# true value for L1
L=L1

g(s) = T-A*exp(-2*a*s)*(1/(1-B*exp(-2*a*L*s)))**(1/L)

set logscale y 10

plot file u 1:(abs($2-g($1))) w l

fit [x_min:x_max] g(x) file u 1:2 every 10 via T,A,B,L

print "L = ", L, "+-", abs(1-L/L1), "(compared to result from eigenproblem)"
