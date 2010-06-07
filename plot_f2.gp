#!/usr/bin/gnuplot -persist

file2="log/info_1/log000_f2.dat"

set terminal epslatex size 7,3
set output "./graphics/Gradient_flow_stages_f2.tex"

unset title

set xlabel '$t$'
set ylabel '$\lvert\partial_t\partial_\psi f(t,0)\rvert$' offset graph .07
unset key
set logscale y
set yrange [:10]
set format y '$10^{%L}$'

set label 'near $f_2$' at 4, graph .85 center
set label 'near $f_0$' at 14, graph .85 center
set arrow from 2,  graph .8 to 6,  graph .8
set arrow from 6,  graph .8 to 2,  graph .8
set arrow from 16, graph .8 to 12, graph .8

plot file2 u 1:(abs($9)) w l

file3="log/info_1/log000_f3.dat"

# set terminal epslatex size 7,3
set output "./graphics/Gradient_flow_stages_f3.tex"

# unset title
# set xlabel '$t$'
# set ylabel '$\lvert\partial_t\partial_\psi f\big|_{\psi=0}\rvert$'
# unset key
# set logscale y
set yrange [:100]
set xrange [:6]

# unset arrow from 2,  graph .8 to 6,  graph .8
# unset arrow from 6,  graph .8 to 2,  graph .8
# unset arrow from 16, graph .8 to 12, graph .8

set label 1 'near $f_3$' at 1.75, graph .85 center
set label 2 'near $f_1$' at 5, graph .85 center
set arrow 1 from 1,  graph .8 to 2.5,  graph .8
set arrow 2 from 2.5,  graph .8 to 1,  graph .8
set arrow 3 from 6, graph .8 to 4, graph .8

plot file3 u 1:(abs($9)) w l
