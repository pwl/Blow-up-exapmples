#!/bin/bash

mrows=2
mcols=3
profiles="./harvester_data/harmonic_k3.00000_l1.0.dat"
# mtot=$(./block.awk $profiles)
mtot=6 #$(grep -c 'A = ' $1)
# snapshot_files=$(find $snapshot_dir -name "$snapshot_name")
startx=0.1
starty=0.1
stopx=0.9
stopy=0.9

# echo "$snapshot_files"
# exit 0

echo "" > plotter.gp
# echo "set size 1,3" >> plotter.gp
echo "set multiplot" >> plotter.gp

for i in $(seq 0 $((mtot-1))); do
    # echo "$i"

    echo "set logscale x" >> plotter.gp

    # setup the tics for picture in the lower left corner
    tics=$(echo "(($i%$mrows)==0 && ($i/$mrows)==$mrows)"|bc)
    if [ $tics -eq 1 ]; then
    	echo "set tics nomirror in" >> plotter.gp
	echo "set xtics 1e-6,100,1" >> plotter.gp
	echo "set ytics 0,pi/4,pi format \"\"" >> plotter.gp
	echo "set ytics add (\"0\" 0, \"\\\$\\\pi/2\\\$\" pi/2,  \"\\\$\\\pi\\\$\" pi)" >> plotter.gp
	echo 'set xlabel "\$\\ln(y)\$" offset 0., screen' ".1*$sizey" >> plotter.gp
	# echo "set xlabel \"\\\$\\\ln(y)\\\$\" offset 0., screen .1*$sizey" >> plotter.gp
	echo 'set ylabel "\$f_n(y)\$" offset screen' ".2*$sizex, 0." >> plotter.gp
	echo 'set format x "\$10^{%L}\$"' >> plotter.gp
    else
	echo "unset tics" >> plotter.gp
	echo "unset xlabel" >> plotter.gp
	echo "unset ylabel" >> plotter.gp
    fi

    echo "set key off" >> plotter.gp
    # switch margins off
    echo "set rmargin 0" >> plotter.gp
    echo "set lmargin 0" >> plotter.gp
    echo "set tmargin 0" >> plotter.gp
    echo "set bmargin 0" >> plotter.gp
    # setupt plot title
    echo "unset title" >> plotter.gp
    # setup plot size and position
    sizex=$(echo "scale=20;($stopx-$startx)/$mrows"|bc)
    sizey=$(echo "scale=20;($stopy-$starty)/$mcols"|bc)
    echo "set size $sizex,$sizey" >> plotter.gp
    orx=$(echo "a=($i%$mrows);scale=20;$startx+a*($stopx-$startx)/$mrows"|bc)
    ory=$(echo "a=($i/$mrows);scale=20;$stopy-(($stopy-$starty)*(a+1))/$mcols"|bc)
    echo "set origin $orx,$ory" >> plotter.gp
    # plot the data
    echo -ne "plot [1.e-7:10] [0:pi] \"$profiles\" index $i every 1 w l lw 2 lc 1 t\"\", pi/2\n" >> plotter.gp
    echo "set title \"\\\$f_$((i+1))\\\$\" offset graph .0, graph -.4; replot" >> plotter.gp
done

echo "unset multiplot" >> plotter.gp

./Shrinker_profiles.gp
# evince snapshot_to_multiplot.ps
