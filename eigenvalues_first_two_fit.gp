#!/usr/bin/gnuplot -persist

file="log/info_1/log000.dat"
file_snap="log/snapshot/snap_1.500018776613762.dat"

FIT_LIMIT=1.e-15
A=1.e-10
B=-1.
lambda1=-4.
lambda2=2.

f(t)=-lambda1*A*exp(-lambda1*t)-lambda2*B*exp(-lambda2*t)


set logscale y
plot file u 2:(abs($8)) w l, abs(f(x))

fit [2.:2.5] f(x) file u 2:8 via A,B,lambda1,lambda2

# plot file u 2:(abs($8)*exp(lambda1*$2)) w l

# plot file_snap u 1:4 w l
