#!/bin/bash

profiles="./harvester_data/harmonic_k3.00000_l1.0.dat"
mtot=5 #$(grep -c 'A = ' $1)
plot="plot "

for i in $(seq 0 $((mtot-1))); do
    # echo "$i"

    # plot the data
    plot="$plot \"$profiles\" index $i w l lw 1 lc 1,"
    # echo "set title \"\\\$f_$((i+1))\\\$\" offset graph .0, graph -.4; replot" >> plotter.gp
done

plot="$plot pi/2"

echo "$plot" > plotter.gp

./Shrinker_profiles.gp
# evince snapshot_to_multiplot.ps
