#!/usr/bin/gnuplot -persist

set output "Shrinker_asymptotics.ps"
set terminal postscript enhanced color

set title "Asymptotic of shrinkers and expanders"


set logscale x
set logscale y
set logscale y2

set y2label "b-{/Symbol p}/2"
set xlabel "a"

set tics nomirror in
set y2tics
unset my2tics

# set arrow nowhead

# plot [ ] [.0001:pi/2] "./Expander_asymptotics.dat" u 1:($2-pi/2) w l t "Expander from 0",\
# "./Expander_asymptotics.dat" u 1:(pi/2-$2) w l t "Expander from {/Symbol p}"

load "plotter.gp"

# set grid ytics noxtics

# replot
