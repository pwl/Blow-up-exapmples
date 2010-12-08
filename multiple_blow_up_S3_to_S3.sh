#!/bin/bash

mplotx=
snapshot_dir="log/snapshot/"
logfile="log/info_1/log000.dat"
snapshot_name="*.dat"
mrows=4
mcols=5
mtot=$((mcols*mrows))
mspace=$((mtot))
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t'_' -k2 | awk 'NR % 365 == 0 && NR >= 0' | head -n$mtot|awk 'NR%1==0 {print}')
# snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
#     sort -n -t'_' -k2 | ./select_by_time.sh .04 | head -n$mtot|awk 'NR%1==0 {print}')
blowup_file1="harvester_data_shrinker/shrinker_k3.00000_l1.0.dat"
# blowup_file2="harvester_data_shrinker/eigen_k3.00000_l1.0_i1.dat"
blowup_file2="harvester_data_expander/expander_k3.00000_l1.0.dat"
startx=0.1
starty=0.1
stopx=0.9
stopy=0.9
size_mult=1.
T=0.328075426868397

echo "" > plotter.gp
echo "set terminal postscript enhanced color size 7,7" >> plotter.gp
echo "set output \"graphics/multiple_blow_up_S3_to_S3.ps\"" >> plotter.gp
# echo "set size $size_mult,$size_mult" >> plotter.gp
echo "set multiplot title \"Passing through the blow-up\"" >> plotter.gp

i=0

for snap in $snapshot_files; do

    t=$(awk '/t = /{print $4}' $snap)
    t=$(awk '/t = /{printf("%.7f\n",$4)}' $snap)
    g=$(awk '/g = /{print $4}' $snap)
    gprint=$(awk '/g = /{printf("%1.2E\n",$4)}' $snap)
    s=$(awk '/s = /{printf("%.0f",$4)}' $snap)
    du=$(awk '/du = /{printf("%.0f",$4)}' $snap)
    T_t=$(awk "BEGIN {printf(\"%.2E\n\",$t-$T);exit 0}")

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
	echo 'set ytics (0, "{/Symbol p}" pi, "2{/Symbol p}" 2*pi, "3{/Symbol p}" 3*pi)' >> plotter.gp
	echo 'set xtics (0, "{/Symbol p}" pi)' >> plotter.gp
	echo "set xlabel \"{/Symbol f}\"" >> plotter.gp
	# echo "set ylabel \"|u(r,t)-f_0(y)|/N\" rotate offset screen .1*$sizex, 0." >> plotter.gp
    fi

    echo "set title \"t=$t\" offset screen .05*$sizex, screen -1.1*$sizey font \"Times-Roman,10\"" >> plotter.gp
    echo -ne "plot [0:pi] [ 0:3*pi ] \"$snap\" u 1:(\$2/sin(\$1)) w l lw 2\n" >> plotter.gp

    i=$((i+1))
done

echo "unset multiplot" >> plotter.gp

./multiple_blow_up_S3_to_S3.gp
ps2pdf graphics/multiple_blow_up_S3_to_S3.ps graphics/multiple_blow_up_S3_to_S3.pdf
evince graphics/multiple_blow_up_S3_to_S3.ps
