#!/usr/bin/gnuplot -persist

set terminal postscript enhanced
set output "graphics/first_mode.ps"

load "plotter.gp"

f(t,ur,urt)=log(abs(.5*(ur-2.*(T-t)*urt))*sqrt(abs(T-t)))
# f(t,ur,urt)=1/2*(ur)
s(t)=-log(abs(T-t))

# plot file u (s($2)):(f($2,$5,$9)) w l

FIT_LIMIT=1.e-15
fit [10:15] A*x+B file u (s($2)):(f($2,$5,$9)) via A,B

# plot file u ($1/50.):(s($2)) w l

# replot A*x+B

L1 = abs(A)
C1 = abs(exp(B)/A)
C1exp_t1 = C1*(T-t1)**L1
C1exp_t2 = C1*(T-t2)**L1

set print "mode_coef.dat"
# print L1
# print C1
# print C1exp_t1
# print T-t1, " ", T-t2
# print -log(T-t1)

# set logscale xy 10
# plot [0:4] snap1 u ($1/sqrt(T-t1)):($1*$3-2.*(T-t1)*$4) every 1 w p pt 8, mod_file u 1:(-2*L1*C1exp_t1*$4) index 1 w l lt 2

set xlabel "y"
set ylabel "f_1(y)-u(t,r)"
set key left top
plot [0:4] snap2 u ($1/sqrt(T-t2)):($1*$3-2.*(T-t2)*$4) every 1 w p pt 8 t "Numerical data",\
     mod_file u 1:(-2*L1*C1exp_t2*$4) index 1 w l lt 2 t "c_1(T-t)^{{/Symbol l}_1}v_1(y)"
