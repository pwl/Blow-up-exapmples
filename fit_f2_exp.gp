#!/usr/bin/gnuplot -persist

FIT_LIMIT=1.e-15

file2="log/info_1/log000_f2.dat"
f2(t)=A0*exp(-L0*t)+A2*exp(-L2*t)
g2(t)=A00*exp(-L00*t)

A0=-1
A2=1.
L0=-1.
L2=1.
L00=1.
A00=-1.e15

# fit [1.5:8] f2(x) file2 u 1:9 via A0,A2,L0,L2
# fit [12:] g2(x) file2 u 1:9 via A00,L00


file3="log/info_1/log000_f3.dat"
f3(t)=A1*exp(-L1*t)+A3*exp(-L3*t)
g3(t)=A11*exp(-L11*t) #+ A13*exp(-L13)

A1=-1.
A3=1.
L1=-5.
L3=1.
A11=-1.e10
L11=1.
A13=-1.e5
L13=4.

plot [] [:10] file3 u 1:(abs($9)) w l, abs(g3(x))
# fit [1.:2.5] f3(x) file3 u 1:9 via A1,A3,L1,L3
fit [5:6] g3(x) file3 u 1:9 via A11,L11

print "L0=", L0
print "L2=", L2
print "L00=", L00
print "L1=", L1
print "L3=", L3
print "L11=", L11

# # set terminal epslatex
# # set output "graphics/Gradient_flow_stages_f2.tex"

# unset title
# set xlabel '$t$'
# set ylabel '$\lvert\partial_t\partial_\psi f\big|_{\psi=0}\rvert$'
# unset key
# set logscale y
# set yrange [:10]

# plot file2 u 1:(abs($9)) w l #, abs(f2(x)), abs(g2(x))
# # plot file2 u 1:9 w l, abs(f2(x)), g2(x)


# # set multiplot

# # set lmargin 0
# # set rmargin 0
# # set tmargin 0
# # set bmargin 0
# # set key off
# # set logscale y

# # set size .8, .4
# # set origin 0.1, .5
# # unset title
# # unset xtics
# # set x2tics

# # plot file1 u 1:(abs($9)) w l

# # set origin 0.1, .1
# # set xtics

# # plot file2 u 1:(abs($9)) w l

# # unset multiplot
