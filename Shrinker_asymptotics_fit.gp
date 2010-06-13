#!/usr/bin/gnuplot -persist

FIT_LIMIT=1.e-15

set print "/dev/null"

data="Shrinker_asymptotics.tmp"
dim=3

f(y)=pi/2+b-(dim-1)/2*sin(2*b)/y**2

b=1.

fit [8:9] f(x) data via b

# plot [2:] data u 1:(abs($2-f($1))) w l

set print "Shrinker_asymptotics.dat" append
print b
set print "-"
print b
exit
