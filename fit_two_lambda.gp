#!/usr/bin/gnuplot -persist

FIT_LIMIT=1.e-15

file="fit_two_lambda.dat"

A1=1.
A2=.1
A3=100.
L1=.5
L2=1.63
L3=2.7

f(y)=log(abs(A1*exp(y*L1)+A2*exp(y*L2))+A3*exp(y*L3))
f(y)=log(abs(A1*exp(y*L1)+A2*exp(y*L2)))
f(y)=log(abs(A1*exp(y*L1)))

plot [-7:] file u (log(abs($1))):(log(abs($2))) w l, f(x)

fit [-7:-4] f(x) file u (log(abs($1))):(log(abs($2))) via A1, L1

# plot file u (log(abs($1))):(log(abs($2))-f(log(abs($1)))) w l

#fit [-10:0] f(x) file u (log(abs($1))):(log(abs($2))) via A1, A2, L1, L2

