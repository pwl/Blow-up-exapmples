#!/bin/bash

snapshot_dir="log/snapshot_f2"
snapshot_name="*.dat"
mrows=3
mcols=3
mtot=$((mcols*mrows))
# snapshot_files=$(find $snapshot_dir -name "$snapshot_name")
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t'_' -k3 | awk '(NR-1) % 145 == 0'| head -n$mtot)
blowup_file1="harvester_data_harmonic/eigen_k3.00000_l1.0_i2.dat"
blowup_file2="harvester_data_harmonic/eigen_k3.00000_l1.0_i2.dat"
startx=0.1
starty=0.1
stopx=0.9
stopy=0.9
xrange="50"
yrange="1.2*pi"
size_mult=1.

# echo "$snapshot_files"
# exit 0

echo "" > plotter.gp
echo "set terminal epslatex color size 7,7" >> plotter.gp
# echo "set size $size_mult,$size_mult" >> plotter.gp
# echo "set multiplot title \"Symetric modes of \\\$f_2\\\$ and \\\$f_0\\\$\"" >> plotter.gp
echo "set multiplot" >> plotter.gp

i=0

for snap in $snapshot_files; do

    t=$(./snap_to_time.sh $snap)
    t=${t:0:3}

    echo "set logscale y 10" >> plotter.gp
    echo "set key off" >> plotter.gp
    # switch margins off
    echo "set rmargin 0" >> plotter.gp
    echo "set lmargin 0" >> plotter.gp
    echo "set tmargin 0" >> plotter.gp
    echo "set bmargin 0" >> plotter.gp
    # setup plot size and position
    sizex=$(echo "scale=20;$size_mult*($stopx-$startx)/$mrows"|bc)
    sizey=$(echo "scale=20;$size_mult*($stopy-$starty)/$mcols"|bc)
    echo "set size $sizex,$sizey" >> plotter.gp
    orx=$(echo "a=($i%$mrows);scale=20;$startx+a*$sizex"|bc)
    ory=$(echo "a=($i/$mrows);scale=20;$stopy-(a+1)*$sizey"|bc)
    echo "set origin $orx,$ory" >> plotter.gp

    # setup the tics for picture in the lower left corner
    echo "unset format" >> plotter.gp
    echo "unset tics" >> plotter.gp
    echo "unset grid" >> plotter.gp
    tics=$(echo "($i%$mrows==0) && (($i/$mcols)+1==$mrows)"|bc)
    # echo "set xtics 0,pi/4,pi/2 format \"\" nomirror" >> plotter.gp
    echo "unset xlabel" >> plotter.gp
    echo "unset ylabel" >> plotter.gp
    if [ $tics -eq 1 ]; then
    	echo "set tics nomirror in" >> plotter.gp
	echo "set xtics 0,pi/4,pi/2 format \"\"" >> plotter.gp

	# echo "set xtics (\"\\\$\\\pi/2\\\$\" pi/2)" >> plotter.gp
	# echo "set ytics (\"\\\$\\\pi/2\\\$\" pi/2)" >> plotter.gp
	echo "set format y \"\\\$10^\{%L\}\\\$\"" >> plotter.gp
	echo "set xtics add (\"\\\$0\\\$\" 0, \"\\\$\\\pi/4\\\$\" pi/4, \"\\\$\\\pi/2\\\$\" pi/2)" >> plotter.gp

	echo "set xlabel \"\\\$\\\psi\\\$\"" >> plotter.gp
	echo "set ylabel \"\\\$|\\\partial_t f(t,\\\psi)|\\\$\" rotate offset screen .2*$sizex, 0." >> plotter.gp
	# echo "set ytics 0,pi/4,pi format \"\"" >> plotter.gp
	# echo "set ytics add (\"0\" 0, \"{/Symbol p}/2\" pi/2,  \"{/Symbol p}\" pi)" >> plotter.gp
    fi

    if [ $i -eq $((mtot-1)) ]; then
	echo "set label \"\\\$\\\vn\{0\}\{2\}\\\$\" at graph 0.7, graph 0.4" >> plotter.gp
	echo "set label \"\\\$\\\vn\{2\}\{2\}\\\$\" at graph 0.6, graph 0.2" >> plotter.gp
    fi

    echo "set title \"\\\$t=$t\\\$\" offset screen -.2*$sizex, screen -1.05*$sizey" >> plotter.gp
    # plot the data
    echo -ne "plot [0:pi/2] [1.e-2:1.e2] \"$snap\" u 1:(abs(\$4)) w l lw 3 lc 1 t\"\"," >> plotter.gp
    echo -ne "\"$blowup_file1\" u 1:(abs(\$4)) index 0 w l lt 3 t\"\", " >> plotter.gp
    echo -ne "\"$blowup_file1\" u 1:(abs(\$4)) index 2 w l lt 3 t\"\"\n" >> plotter.gp
    # echo -ne "\"$blowup_file2\" u 1:(abs(\$4)) w l lt 3 t\"\"\n" >> plotter.gp

    echo "unset logscale xy" >> plotter.gp
    echo "unset xlabel; unset ylabel; unset tics; unset title; unset label" >> plotter.gp
    orx=$(echo "scale=20;$orx+.6*$sizex"|bc)
    ory=$(echo "scale=20;$ory+.6*$sizey"|bc)
    echo "set origin $orx,$ory" >> plotter.gp
    sizex=$(echo "scale=20;.4*$sizex"|bc)
    sizey=$(echo "scale=20;.4*$sizey"|bc)
    echo "set size $sizex,$sizey" >> plotter.gp

    # tics=$(echo "($i%$mrows==0) && (($i/$mcols)+1==$mrows)"|bc)
    if [ $i -eq 0 ]; then
    	echo "set tics nomirror in" >> plotter.gp
	# echo "set xtics 0,pi/2,pi format \"\"" >> plotter.gp
	echo "set xtics (\"\\\$\\\pi/2\\\$\" pi/2)" >> plotter.gp
	echo "set ytics (\"\\\$\\\pi/2\\\$\" pi/2)" >> plotter.gp
	echo "set grid" >> plotter.gp
    fi

    echo "plot [0:pi] [0:pi] \"$snap\" w l lt 1" >> plotter.gp
    # echo "\"\" u (pi-\$1):(pi-\$2) w l lt 1" >> plotter.gp

    i=$((i+1))
done

echo "unset multiplot" >> plotter.gp

./snapshot_f2.gp
ps2pdf graphics/snapshot_f2.eps graphics/snapshot_f2.pdf
# evince snapshot_to_multiplot.ps
