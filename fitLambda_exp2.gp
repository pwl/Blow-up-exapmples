#!/usr/bin/gnuplot -persist

x_min = 250
x_max = 1000
file = "log/info_1/log000.dat"
FIT_LIMIT = 1.e-20

B=-1.5
T=0.33
A=2.
a=0.01
L1_REAL=0.51762191809152735544	# true value for L1
L2_REAL=1.630548978939606
L1=-2.
L2=1.
A0=-3.
A1=-10.
A2=13.5386
A3=1.
A4=3.

flim(L)=(atan(L)+pi/2.)/1.5
# fliminv(L)=tan(L-pi/2.)

f1(s)=A0+A1*exp(-s*flim(L1))+A2*exp(-2.*s*flim(L1))#+A3*exp(-3.*s*L1)+A4*exp(-s*L2)#+A5*exp(-4*s*L1)+A6*exp(-s*(L1+L2))
f2(s)=A0+A1*exp(-s*flim(L1))+A2*exp(-2.*s*flim(L1))+A3*exp(-3.*s*flim(L1))+A4*exp(-s*L2)# +A5*exp(-4*s*flim(L1))+A6*exp(-s*(flim(L1)+flim(L2)))
f3(s)=A0+A1*exp(-s*flim(L1))+A2*exp(-2.*s*flim(L1))+A3*exp(-3.*s*flim(L1))+A4*exp(-s*L2)+A5*exp(-4*s*flim(L1))+A6*exp(-s*(flim(L1)+flim(L2)))

# set logscale y

# fit [50:] f(x/100.) file u 1:(exp(-$1/100.)*$5) every 100 via A0,A1,A2,A3,A4,L1,L2
fit [300:] f1(x/50.) file u 1:($5*sqrt($6/0.02)) every 100 via A0,A1,A2,L1
fit [80:] f2(x/50.) file u 1:($5*sqrt($6/0.02)) every 100 via A0,A1,A2,A3,A4,L1,L2
# fit [50:] f2(x/50.) file u 1:($5*sqrt($6/0.02)) every 100 via A0,A1,A2,A3,A4,A5,A6,L1,L2


# plot file u 1:(abs($5*sqrt($6/0.02)-f1($1/50.))) every 100 w l
plot file u 1:(abs($5*sqrt($6/0.02)-f2($1/50.))) every 100 w l
print flim(L1), "  ",  flim(L2)
# plot file u 1:5 w l

# # plot file u 1:(abs($2-g3($1))) w l

# fit [900:x_max] g1(x) file u 1:2 every 10 via T,A,A1,L1
# fit [600:x_max] g2(x) file u 1:2 every 2 via T,A,A1,A2,L1
# # L1=0.51762191809152735544	# true value for L1
# plot file u 1:(abs(g2($1)-$2)) w l
# set print "fit.dat"
# print "L1 = ", L1, " +- ", abs(1-L1/L1_REAL)
# fit [100:400] log(g4(x)) file u 1:(log(abs(g2($1)-$2))) every 2 via A3,L2
# replot file u 1:(abs(g3($1)-$2)) w l, g4(x)

# # print "L = ", L, "+-", abs(1-L/L1), "(compared to result from eigenproblem)"
