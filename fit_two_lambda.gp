#!/usr/bin/gnuplot -persist

FIT_LIMIT=1.e-15

load "plotter.gp"

f(t,ur,urt)=log(abs(.5*(ur-2.*(T-t)*urt))*sqrt(abs(T-t)))
# f(t,ur,urt)=1/2*(ur)
s(t)=-log(abs(T-t))

plot file u (s($2)):(f($2,$5,$9)) w l

fit [35:] A*x+B file u (s($2)):(f($2,$5,$9)) via A,B

replot A*x+B
