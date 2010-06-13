#!/usr/bin/gnuplot -persist

FIT_LIMIT=1.e-20

file="harvester_data/static.dat"

d=3				# dimension
omega=sqrt(8*d-d**2-8)/2
c=1.
delta=pi/2
f(x)=pi/2+c*(x**(-(d-2)/2.))*sin(omega*log(x)+delta)
f(x)=c*sin(omega*log(x)+delta)

# plot [2:100] g(exp(x))
# plot file u 1:($1**((d-2)/2.)*($2-pi/2.)) w l, f(x)
fit [1.e10:] f(x) file u 1:($1**((d-2)/2.)*($2-pi/2.)) via c,delta
# fit [100:] f(x) file via c,delta
# fit [1000:] f(x) file via c,delta
# fit [10000:] f(x) file via c,delta

set print "Rk_static_fit.dat"
print "c=", c
print "delta=", delta
