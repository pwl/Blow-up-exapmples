#!/usr/bin/gnuplot -persist

data="Expander_asymptotics.dat"

f(t)=A*exp(-alpha*t)*sin(omega*t+phi)

omega=sqrt(7)/2
alpha=.5
A=.22
phi=-.35

fit [7:] f(x) data u (log($1)):($2-pi/2) via A,phi,omega,alpha

plot [4:] data u (log($1)):(abs(f(log($1))-$2+pi/2))

# plot [0:] f(x), data u (log($1)):($2-pi/2) w l
