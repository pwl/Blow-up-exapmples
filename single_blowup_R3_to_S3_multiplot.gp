set terminal eps monochrome
set output "graphics/single_blowup_R3_to_S3_multiplot.eps"
set nodisplay

gold_ratio=(1+sqrt(5)/2)
width=5.4
height=1/gold_ratio*width
b1=pi/2+0.573141133043885
lposx=1.6
lposy=.3

set size width, height

set log x1 10
set x1format ""
set y1format ""
set nokey
set noy1tics
set x1range [1e-2:1e4]
set y1range [0:pi]
set x1tics (1e-2, 1, 1e2, 1e4)
set y1tics (0, b1 , pi)

set multiplot

load "plotter.gp"

# setup the first plot
set item 7 axis x bottom
set item 7 x1label "$y$"
set item 7 y1label "$u(t,r)$"
set item 7 x1tics ("$10^{-2}$" 1e-2, "$1$" 1, "$10^2$" 1e2, "$10^4$" 1e4)
# set item 1 x1format "$10^{%s}$"%(log(x)/log(10))
set item 7 y1tics ("0" 0, "$b_1$" b1, "$\pi$" pi)
set item 7 grid y1
set item 1 label 1 "\footnotesize{$T-t=%s$}"%(T_t1) graph lposx, graph lposy

set display

refresh
