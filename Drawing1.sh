#!/bin/bash

klist={3.0,4.0,5.0,6.0}
files=$(eval ls -1 harvester_data/common_data_k$klist*.dat)
filesBn0=$(eval ls -1 harvester_data/Zeroes_of_expander_asymptotics_k$klist*.dat)
tempfile=$(tempfile)

eval cat harvester_data/Zeroes_of_expander_asymptotics_k$klist*.dat > $tempfile
# less $tempfile

for file in $files; do

    bn=$(awk 'NR>1{print $4}' $file)
    blist=""
    dim=$(awk 'NR==2 {print $1}' $file)
    dimint=$(awk 'NR==2 {printf("%.0f", $1)}' $file)
    plot=""
    # fileBn0=$(awk "NR==$j+1 {print}" $filesBn0)

    echo "" > plotter.gp
    i=1
    for b in $bn; do
	# Bn0=$(cat $tempfile | awk "$dim==\$1 && $i==\$3 {print \$4}")
	# An0=$(cat $tempfile | awk "$dim==\$1 && $i==\$3 {print \$5}")
	# exit 0
	mod=$(echo "$i%2"|bc)
	sign=""
	if [ $mod -eq 0 ]; then sign="-"; fi
	# echo "set label \"${sign}b_$i\" at 10.**(-3.+$i/2.5), abs($b) offset 0, graph .024 font \"Helvatica,18\"" >> plotter.gp
	plot="$plot, $b*x**(($dim-2.)/2.) lt -1 lw .5"
	i=$((i+1))
    done

    # echo "set yrange [abs($b)/exp(($dim-2)/2.):]" >> plotter.gp
    # echo "set xrange [:7.e7*10**(($dim-6))]" >> plotter.gp

    echo "set label \"d=$dimint\" at graph .8, graph .9" >> plotter.gp
    # plot=", \"\" u 1:(pi/2-\$2) w l lt 2 lw 3 $plot"
    # plot="plot file u 1:(\$2-pi/2.) w l lt 1 lw 3 $plot"
    plot=", \"\" u 1:(-\$2*\$1**(($dim-2.)/2.)) w l lt 2 lw 3 $plot"
    plot="plot file u 1:(\$2*\$1**(($dim-2.)/2.)) w l lt 1 lw 3 $plot"
    echo "blist=\"$blist\"" >> plotter.gp
    echo "set terminal png enhanced size 800,600" >> plotter.gp
    echo "set output \"graphics/roots_k$dim.png\"" >> plotter.gp
    # echo "set terminal postscript eps enhanced color font 20 solid" >> plotter.gp
    # echo "set output \"graphics/roots_k$dim.eps\"" >> plotter.gp
    echo "dim=$dim" >> plotter.gp
    echo "file=\"Expander_asymptotics_k$dim.dat\"" >> plotter.gp
    echo "$plot" >> plotter.gp

    ./Drawing1.gp

done
