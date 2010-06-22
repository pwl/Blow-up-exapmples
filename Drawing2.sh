#!/bin/bash

# files=$(ls -1 harvester_data/common_data_k{2.1,3.0,4.0,5.0,6.0}*.dat)
files=$(ls -1 harvester_data/common_data_k2.20.dat)

for file in $files; do

    bn=$(awk '{print $4}' $file)
    blist=""
    dim=$(awk 'NR==2 {print $1}' $file)
    dimint=$(awk 'NR==2 {printf("%.1f", $1)}' $file)
    plot=""

    echo "" > plotter.gp
    i=1
    for b in $bn; do
	# b="abs(b)"
	# echo "set label \"${sign}b_$i\" at 10.**(-3.+$i/2.5), abs($b) offset 0, graph .024 font \"Helvatica,18\"" >> plotter.gp
	echo "set label \"b_$i\" at 10.**(-2.5+$i/4.), $b+pi/2 offset 0, graph .024 font \"Helvatica,18\"" >> plotter.gp
	plot="$plot, $b+pi/2 lt -1 lw .5"
	i=$((i+1))
    done

    echo "set label \"d=$dimint\" at graph .8, graph .9" >> plotter.gp
    plot=", \"\" u 1:(pi-\$2) w l lt 2 lw 3 $plot"
    plot="plot file w l lt 1 lw 3 $plot"
    echo "set terminal postscript eps enhanced color font 20 solid" >> plotter.gp
    echo "set output \"graphics/roots_k$dim.eps\"" >> plotter.gp
    echo "dim=$dim" >> plotter.gp
    echo "file=\"Expander_asymptotics_k$dim.dat\"" >> plotter.gp
    echo "$plot" >> plotter.gp

    ./Drawing2.gp

done
