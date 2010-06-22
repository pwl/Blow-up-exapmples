#!/usr/bin/gnuplot -persist

file="./Shrinker_asymptotics.dat"
dim=3.
omega=sqrt(8*dim-8-dim*dim)/2
d1=-0.00928848151332049
c1=0.695220830587285
d=1.80925718904119
c=-0.700472925187306
C=exp((d1-d)/omega)
D=c/c1*C**(-(dim-2)/2)
aratio=1/exp(pi/omega)
bratio=-exp(pi/2./omega)

an(n)=C*exp(n*pi/omega)
bn(n)=D*exp(-n*pi/omega/2*(dim-2))

set logscale xy
set term epslatex
set output "graphics/an_bn_plot.tex"


# set multiplot

set xlabel '$a_n$'
set ylabel '$\lvert b_n\rvert$' offset screen .1
set trange [.1:15]
set xrange [.1:1.e14]
set yrange [1.e-7:10.]
set parametric
set format x '$10^{%L}$'
set format y '$10^{%L}$'
set label '$n=10$' at graph .85, graph .2
set label '$n=1$' at graph .1, graph .9

plot file u 2:(-(-1)**$1*$3):1 ps 2 t "Numerics", an(t),abs(bn(t)) t "Theory"
# set parametric
