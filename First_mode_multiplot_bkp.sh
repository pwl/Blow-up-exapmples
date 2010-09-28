#!/bin/bash

mplotx=
snapshot_dir="log/snapshot"
snapshot_name="*.dat"
mrows=3
mcols=3
mtot=$((mcols*mrows))
mspace=$((mtot))
# snapshot_N=$(find $snapshot_dir -name "$snapshot_name"|wc -l)
# snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
#     sort -n -t'_' -k2 | awk '(NR-1) % 1200 == 0'| head -n$mtot)
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t'_' -k2 | awk 'NR % 1200 == 0'| tail -n$mtot)
blowup_file1="harvester_data_shrinker/shrinker_k3.00000_l1.0.dat"
blowup_file2="harvester_data_shrinker/eigen_k3.00000_l1.0_i1.dat"
startx=0.1
starty=0.1
stopx=0.9
stopy=0.9
xrange="50"
yrange="1.2*pi"
size_mult=1.

tempfile1="test1.dat" #$(tempfile)
tempfile2="test2.dat" #$(tempfile)
tempfile3="test3.dat" #$(tempfile)

# select the first possible shrinker and
./extract_block.awk -v block=1 $blowup_file1 | awk '/^[0-9]/ {print}' > $tempfile1

# echo "$snapshot_files"
# less $tempfile1
# exit 0

echo "" > plotter.gp
echo "set terminal postscript enhanced color size 7,7" >> plotter.gp
echo "set output \"graphics/snapshot_to_multiplot.ps\"" >> plotter.gp
# echo "set size $size_mult,$size_mult" >> plotter.gp
echo "set multiplot title \"Blow-up profile (moving mesh method)\"" >> plotter.gp

i=0

for snap in $snapshot_files; do

    tempfile2=$(tempfile)
    tempfile3=$(tempfile)

    t=$(awk '/t = /{print $4}' $snap)
    g=$(awk '/g = /{print $4}' $snap)
    s=$(awk '/s = /{printf("%.0f",$4)}' $snap)
    du=$(awk '/du = /{printf("%.0f",$4)}' $snap)

    awk '/g = / {g=$4} /^[0-9]/ {printf("%.15E %.15E\n", $1/sqrt(g), $2)}' $snap > $tempfile2
    ./interpolate_at_point.sh $tempfile1 $tempfile2 | join $tempfile2 - 2> /dev/null > $tempfile3
    norm=$(awk '{printf("%.15E",($2-$3)/$1); exit}' $tempfile3)

    echo $norm

    # echo "set logscale y 10" >> plotter.gp
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
	# echo "set xtics 0,pi/4,pi/2 format \"\"" >> plotter.gp

	# echo "set xtics (\"\\\$\\\pi/2\\\$\" pi/2)" >> plotter.gp
	echo "set ytics (0,2,4)" >> plotter.gp
	echo "set format x \"10^\{%L\}\"" >> plotter.gp
	# echo "set format x2 \"10^\{%L\}\"" >> plotter.gp
	# echo "set format y \"10^\{%L\}\"" >> plotter.gp
	# echo "set xtics add (\"\\\$0\\\$\" 0, \"\\\$\\\pi/4\\\$\" pi/4, \"\\\$\\\pi/2\\\$\" pi/2)" >> plotter.gp
	echo "set xlabel \"r*g(t)\"" >> plotter.gp
	echo "set ylabel \"u(r,t)\" rotate offset screen .1*$sizex, 0." >> plotter.gp
	# echo "set ytics 0,pi/4,pi format \"\"" >> plotter.gp
	# echo "set ytics add (\"0\" 0, \"{/Symbol p}/2\" pi/2,  \"{/Symbol p}\" pi)" >> plotter.gp
    fi

    # if [ $i -eq $((mtot-1)) ]; then
    # 	echo "set label \"\\\$\\\vn\{1\}\{3\}\\\$\" at graph 0.3, graph 0.8" >> plotter.gp
    # 	echo "set label \"\\\$\\\vn\{3\}\{3\}\\\$\" at graph 0.1, graph 0.62" >> plotter.gp
    # fi

    echo "set title \"s=$s\" offset screen -.2*$sizex, screen -.25*$sizey" >> plotter.gp
    # plot the data
    echo -ne "plot [.01:] \"$tempfile3\" u 1:(abs((\$2-\$3)/$norm)) t\"\" w l," >> plotter.gp
    # echo -ne "\"$snap\" u (\$1/sqrt($g)):(abs(2.*\$4*\$5)) every ::::30 t\"\"," >> plotter.gp
    echo -ne "\"$blowup_file2\" u 1:(abs(\$4)) index 1 w l lt 2 t\"\"\n" >> plotter.gp
    # echo -ne "\"$blowup_file2\" u 1:(abs(\$4-\$1*\$3/2.738753125884604)/1.1e2) index 1 w l lt 2 t\"\"\n" >> plotter.gp
    # echo -ne "x w l lt 2 t\"\"\n" >> plotter.gp
    # echo -ne "\"$blowup_file2\" u 1:(abs(\$4)) w l lt 3 t\"\"\n" >> plotter.gp

    # echo "plot [1.e-5:] [0:pi] \"$snap\" w l lt 1" >> plotter.gp
    # echo "\"\" u (pi-\$1):(pi-\$2) w l lt 1" >> plotter.gp
# exit

    i=$((i+1))
done

echo "unset multiplot" >> plotter.gp

./snapshot_to_multiplot.gp
ps2pdf graphics/snapshot_to_multiplot.ps graphics/snapshot_to_multiplot.pdf
evince graphics/snapshot_to_multiplot.ps
