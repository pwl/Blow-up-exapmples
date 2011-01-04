#!/usr/bin/gnuplot -persist


set nokey
set term png monochrome antialias dpi 200
load plotter.gp

# plot 1

set xlabel "$\log(\tan(\theta/2))$"
set ylabel "$U(t,\theta)$"
set xrange [-10:10]
set yrange [0:3*pi]
set ytics (0, "$\pi$" pi, "$2\pi$" 2*pi, "$3\pi$" 3*pi)
set grid y
lposx=3.
lposy=1.

set output "movie_1/%010i.png"%(i)

set label 1 "$t=%.3f$"%(t) lposx, lposy
plot f u (log(tan($1/2))):2 every ::1 w l

# plot 2


# height=width
lposx=-.3
lposy=-1.4

set size square #width, height
set trange [ 0:2*pi ]
set noaxis x y
set nokey
set notics
set nogrid
set xrange [-1.6:1.6]
set yrange [-1.6:1.6]

set output "movie_2/%010i.png"%(i)

set label 1 "$t=%.3f$"%(t) lposx, lposy
plot parametric sin(t):cos(t) w l lw 1 lt 2, f u ((1.+.3*sin($1))*sin($2)):((1.+.3*sin($1))*cos($2)) w l lw 2 lt 1
