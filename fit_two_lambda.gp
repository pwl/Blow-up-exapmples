#!/usr/bin/gnuplot -persist

set terminal eps monochrome
load "plotter.gp"

f(t,ur,urt)=log(abs(.5*(ur-2.*(T-t)*urt))*sqrt(abs(T-t)))
# f(t,ur,urt)=1/2*(ur)
s(x)=-log(abs(T-x))

FIT_LIMIT=1.e-15
A=1.
B=1.
g(x)=A*x+B

fit [10:15] g(x) withouterrors file u (s($2)):(f($2,$5,$9)) every 500 via A,B

set xlabel "$-\ln(T-t)$"
set ylabel "$\ln\left(c_1(T-t)^{\lambda_1}\right)$"
set key right top

set output "graphics/first_mode_fit.eps"

plot [10:15] file u (s($2)):(f($2,$5,$9)) every 2500 w p pt 8 ps .7 t "Numerical data"
replot A*x+B w l lt 2 t "Linear fit"

L1 = abs(A)
C1 = abs(exp(B)/A)
C1exp_t1 = C1*(T-t1)**L1
C1exp_t2 = C1*(T-t2)**L1

# set print "mode_coef.dat"
print L1
print C1
print C1exp_t1
print T-t1, " ", T-t2
print -log(T-t1)

set xlabel "$y$"
set ylabel "$f_1(y)-u(t,y\sqrt{T-t})$"
set key left bottom

set output "graphics/first_mode.eps"

plot [0:4] snap2 u ($1/sqrt(T-t2)):(-($1*$3-2.*(T-t2)*$4)) every 3 w p pt 8 ps .7 t "Numerical data",\
     mod_file u 1:(2*L1*C1exp_t2*$4) index 1 w l lt 2 t "$c_1(T-t)^{\lambda_1}v_1(y)$"

