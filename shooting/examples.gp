#!/usr/bin/gnuplot -persist

set terminal postscript
set output "examples.ps"

set logscale x 10
set title "Forward blow-up profiles"

plot "profile_df=-1E+00.dat" w l,\
"profile_df=-1E+01.dat" w l,\
"profile_df=-1E+02.dat" w l,\
"profile_df=-1E+03.dat" w l,\
"profile_df=-1E+04.dat" w l, pi/2

set output "f_100.ps"

set logscale y 10
set title "Forward blow-up profile at x=100"

plot "test2.dat" w l t "|f(100)-pi/2|", pi/2
