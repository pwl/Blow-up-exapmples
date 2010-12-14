#!/bin/bash

mplotx=
snapshot_dir="log/snapshot_multiple_blow-up_S3_S3/"
logfile="log/info_1/log000_multiple_blow-up_S3_S3.dat"
snapshot_name="*.dat"
mrows=4
mcols=4
mtot=$((mcols*mrows))
mspace=$((mtot))
# snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
#     sort -n -t'_' -k2 | awk 'NR % 365 == 0 && NR >= 0' | head -n$mtot|awk 'NR%1==0 {print}')
# snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
#     sort -n -t'_' -k2 | ./select_by_time.sh .04 | head -n$mtot|awk 'NR%1==0 {print}')
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t'_' -k2 | ./select_by_time.sh .06 | head -n$mtot|awk 'NR%1==0 {print}')
startx=0.1
starty=0.1
stopx=0.9
stopy=0.9
size_mult=1.
T=0.328075426868397

echo "
set trange [ 0:2*pi ];\
set nokey;\
set noxtic; set noytic;\
unset border;\
unset grid;\
set xrange [-1.6:1.6];\
set yrange [-1.6:1.6];\
" > plotter.gp

echo "set terminal postscript enhanced size 7,7" >> plotter.gp
echo "set output \"graphics/Passing_through_the_blow_up_cartoon.ps\"" >> plotter.gp
# echo "set size $size_mult,$size_mult" >> plotter.gp
echo "set multiplot title #\"Passing through the blow-up\"" >> plotter.gp

i=0

for snap in $snapshot_files; do

    t=$(awk '/t = /{print $4}' $snap)
    t=$(awk '/t = /{printf("%.2f\n",$4)}' $snap)
    g=$(awk '/g = /{print $4}' $snap)
    gprint=$(awk '/g = /{printf("%1.2E\n",$4)}' $snap)
    s=$(awk '/s = /{printf("%.0f",$4)}' $snap)
    du=$(awk '/du = /{printf("%.0f",$4)}' $snap)
    T_t=$(awk "BEGIN {printf(\"%.2E\n\",$t-$T);exit 0}")

    # echo "set logscale y 10" >> plotter.gp
    echo "set parametric" >> plotter.gp
    echo "set key off" >> plotter.gp
    # switch margins off
    echo "set rmargin 0" >> plotter.gp
    echo "set lmargin 0" >> plotter.gp
    echo "set tmargin 0" >> plotter.gp
    echo "set bmargin 0" >> plotter.gp
    # setup plot size and position
    sizex=$(echo "scale=20;$size_mult*($stopx-$startx)/$mrows"|bc)
    sizey=$(echo "scale=20;$size_mult*($stopy-$starty)/$mcols"|bc)
    echo "set size square .2,.2" >> plotter.gp
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

    echo "set title \"t = $t\" offset screen .06*$sizex, screen -.85*$sizey font \"Times-Roman,10\"" >> plotter.gp
    echo -ne "plot sin(t),cos(t) lw 1 lt 2, \"$snap\" u ((1.+.3*sin(\$1))*sin(\$2/sin(\$1))):((1.+.3*sin(\$1))*cos(\$2/sin(\$1))) w l lw 2 lt 1\n" >> plotter.gp

    i=$((i+1))
done

echo "unset multiplot" >> plotter.gp

gnuplot plotter.gp


ps2pdf graphics/Passing_through_the_blow_up_cartoon.ps graphics/Passing_through_the_blow_up_cartoon.pdf
evince graphics/Passing_through_the_blow_up_cartoon.ps
