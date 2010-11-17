#!/usr/bin/gnuplot -persist

FIT_LIMIT=1.e-16

file="log/info_1/log000.dat"

alpha=0.02

t(s)	=(s<=sc)?(A*(1-exp(alpha*(sc-s)))+T):(A*(exp(alpha*(s-sc))-1)+T)
dt(s)	=A*(exp(alpha*abs(sc-s)))
# dt(s)	=A*(exp(alpha*(sc-s)))

sc	=1200.
A	=1.e-13

# plot [900:1300] file u 1:(log($6)) every 10 w l, log(dt(x))


fit [900:1300] log(A)+alpha*abs(sc-x) file u 1:(log($6)) every 100 via A,sc

fit [900:1300] t(x) file u 1:2 every 100 via A,sc,T

plot [900:1300] file u 1:(abs($2/t($1)-1)) every 10 w l

print T
