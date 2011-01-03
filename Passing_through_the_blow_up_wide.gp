set terminal eps monochrome
set fontsize 1.3
set output "graphics/Passing_through_the_blow_up_wide.eps"
set nodisplay

gold_ratio=(1+sqrt(5)/2)
width=5.4
height=.8*width
lposx=1.6
lposy=.2

set size width, height
set nokey
set axis x1 bottom
set x1format ""
set y1format ""
# set x1range [-15:15]
set x1range [0:pi]
set y1range [0:3*pi]
set x1tics (0, pi/2, pi)
set y1tics (0, pi, 2*pi, 3*pi)
set grid y1

set multiplot

load plotter.gp

# set item 1 label 1 "$t=%.2f$"%(t1) graph lposx-.3, graph lposy
set item 1 label 1 "$t=0$" graph lposx-.3, graph lposy
set item 7 y1tics ("$0$" 0, "$\pi$" pi, "$2\pi$" 2*pi, "$3\pi$" 3*pi)
set item 7 x1tics (0, "$\pi/2$" pi/2, "$\pi$" pi)
unset item 7 x1format
set item 7 xlabel "$\theta$"
set item 7 ylabel "$U(t,\theta)$"

set display

refresh

clear

set output "graphics/Passing_through_the_blow_up_cartoon.eps"
set nodisplay

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

load plotter2.gp

# set item 1 label 1 "$t=%.2f$"%(t1) graph lposx-.3, graph lposy
set item 1 label 1 "$t=0$" graph lposx-.3, graph lposy

set display

refresh

unset multiplot
