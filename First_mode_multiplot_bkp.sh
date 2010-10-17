#!/bin/bash

mplotx=
snapshot_dir="log/snapshotN150_rkf45_te-14/"
logfile="log/info_1/logN150_rkf45_te-14.dat"
snapshot_name="*.dat"
mrows=3
mcols=3
mtot=$((mcols*mrows))
mspace=$((mtot))
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t'_' -k2 | awk 'NR % 24 == 0'| tail -n$mtot|awk 'NR%1==0 {print}')
blowup_file1="harvester_data_shrinker/shrinker_k3.00000_l1.0.dat"
blowup_file2="harvester_data_shrinker/eigen_k3.00000_l1.0_i1.dat"
startx=0.1
starty=0.1
stopx=0.9
stopy=0.9
xrange="50"
yrange="1.2*pi"
size_mult=1.
T=$(awk 'BEGIN {max=0} {if($2 > max) max=$2} END {printf("%.15E\n",max)}' $logfile)

tempfile1=$(tempfile)
tempfile2=$(tempfile)
tempfile2="temp2.dat"
tempfile3=$(tempfile)
tempfile3="temp3.dat"

# select the first possible shrinker and
./extract_block.awk -v block=1 $blowup_file1 | awk '/^[0-9]/ {print}' > $tempfile1

# echo "$snapshot_files"
# # less $tempfile1
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
    # T_t=$(echo "scale=20; $T-$t|bc")

    # awk '/g = / {g=$4} /^[0-9]/ {printf("%.20E %.20E\n", $1/sqrt(g), $2)}' $snap > $tempfile2
    awk "/t = / {t=\$4} /^[0-9]/ {printf(\"%.20G %.20G\\n\", \$1/sqrt($T-t+1.e-8), \$2)}" $snap > $tempfile2
    ./interpolate_at_point.sh $tempfile1 $tempfile2 | join $tempfile2 - 2> /dev/null > $tempfile3
    # TODO: norm!
    norm=$(awk 'NR == 6 {printf("%.20G",($2-$3)/$1); exit}' $tempfile3)
    # norm=$(awk 'NR == 7 {dx=($2-$3)/$1} {printf("%.15E",($2-$3)/dx); exit}' $tempfile3)

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
    # echo "unset format" >> plotter.gp
    echo "unset tics" >> plotter.gp
    # echo "set grid" >> plotter.gp
    tics=$(echo "($i%$mrows==0) && (($i/$mcols)+1==$mrows)"|bc)
    echo "unset xlabel" >> plotter.gp
    echo "unset ylabel" >> plotter.gp
    if [ $tics -eq 1 ]; then
    	echo "set tics nomirror in" >> plotter.gp
	echo "set ytics (0,2,4)" >> plotter.gp
	echo "set xlabel \"y\"" >> plotter.gp
	echo "set ylabel \"|u(r,t)-f_0(y)|/N\" rotate offset screen .1*$sizex, 0." >> plotter.gp

    fi

    echo "set title \"t=$t\" offset screen -.15*$sizex, screen -.25*$sizey font \"Times-Roman,10\"" >> plotter.gp
    echo -ne "plot [:10] [-1.8:.5]\"$tempfile3\" u (\$1):(((\$2-\$3)/$norm)) t\"\" w l," >> plotter.gp
    echo -ne "\"$blowup_file2\" u 1:((\$4)) index 1 w l lt 2 t\"\", 0 t\"\"\n" >> plotter.gp

    i=$((i+1))
done

echo "unset multiplot" >> plotter.gp

./snapshot_to_multiplot.gp
ps2pdf graphics/snapshot_to_multiplot.ps graphics/snapshot_to_multiplot.pdf
evince graphics/snapshot_to_multiplot.ps
