#!/usr/bin/gnuplot -persist

set macro

FIT_LIMIT=1.e-50

A0=2.857
A1=5.e-10
A2=-1
L1=1
L2=-.5

file="log/info_1/log000.dat"
f(t)=A0+A1*exp(L1*t)+A2*exp(L2*t)
FILE="\"log/info_1/log000.dat\" u ($1/100):($5/10)"

plot [5:] @FILE w l, f(x)
fit [5:] f(x) @FILE via A0,A1,A2,L1,L2
replot

set print "fitApproach_sy.dat"
print "A1=",A1
print "A2=",A2
