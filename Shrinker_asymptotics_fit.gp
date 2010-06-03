#!/usr/bin/gnuplot -persist

FIT_LIMIT=1.e-15

set print "/dev/null"

data="Shrinker_asymptotics.tmp"

f(y)=b+c/y**2

b=pi/2
c=sin(b)

fit [8:9] f(x) data via b,c

# plot [2:] data u 1:(abs($2-f($1))) w l

set print "Shrinker_asymptotics.dat" append
print b
set print "-"
print b
exit
