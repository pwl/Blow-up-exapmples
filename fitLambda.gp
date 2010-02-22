#!/usr/bin/gnuplot -persist

x_min = 250
x_max = 1000
file = "log/info_1/log000.dat"
FIT_LIMIT = 1.e-20

B=-1.5
T=0.33
A=2.
a=0.01
L1_REAL=0.51762191809152735544	# true value for L1
L2_REAL=1.630548978939606
L1=L1_REAL
L2=3.
A1=-3.
A2=-13.
A3=500.

# g(s) = T-A*exp(-2*a*s)*(1/(1-B*exp(-2*a*L*s)))**(1/L)
g1(s)= T-A*exp(-2*a*s)*(1+A1*exp(-2*a*L1*s))
g2(s)= T-A*exp(-2*a*s)*(1+A1*(exp(-2*a*L1*s)+A2*exp(-4*a*L1*s)))
g3(s)= T-A*exp(-2*a*s)*(1+A1*(exp(-2*a*L1*s)+A2*exp(-4*a*L1*s)+\
A2*A2*exp(-6*a*L1*s))+A3*exp(-2*a*L2*s))
g4(s)=g3(s)-g2(s)

set logscale y 10

# plot file u 1:(abs($2-g3($1))) w l

fit [900:x_max] g1(x) file u 1:2 every 10 via T,A,A1,L1
fit [600:x_max] g2(x) file u 1:2 every 2 via T,A,A1,A2,L1
# L1=0.51762191809152735544	# true value for L1
plot file u 1:(abs(g2($1)-$2)) w l
set print "fit.dat"
print "L1 = ", L1, " +- ", abs(1-L1/L1_REAL)
fit [0:500] g4(x) file u 1:(abs(g2($1)-$2)) every 10 via A3,L2
plot file u 1:(abs(g3($1)-$2)) w l

# print "L = ", L, "+-", abs(1-L/L1), "(compared to result from eigenproblem)"
