#!/usr/bin/gnuplot -persist

set macro

FIT_LIMIT=1.e-50

A0=2.756
A1=1.e-9
A2=-1.3
L1=1
L2=-.511

file="log/info_1/log000.dat"
f(t)=A0+A1*exp(L1*t)+A2*exp(L2*t)
FILE="\"log/info_1/log000.dat\" u ($1/100):($5/50)"

# plot  @FILE w l, f(x)
fit [10:12] f(x) @FILE via A0,A1,A2,L1,L2
plot file u ($1/100):(abs($5/50-f($1/100))) w l
# replot

set print "fitApproach_sy.dat"
print "A1=",A1
print "A2=",A2
