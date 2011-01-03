set terminal eps monochrome
set fontsize 1.3
set output "graphics/Passing_through_the_blow_up_cartoon.eps"
set nodisplay

gold_ratio=(1+sqrt(5)/2)
width=5.4
height=width
lposx=1.6
lposy=.2

set size width, height
set trange [ 0:2*pi ]
set noaxis x y
set nokey
set notics
set nogrid
set xrange [-1.6:1.6]
set yrange [-1.6:1.6]


set multiplot

load plotter.gp

set item 1 label 1 "$t=%s$"%(t1) graph lposx-.3, graph lposy

set display

refresh
