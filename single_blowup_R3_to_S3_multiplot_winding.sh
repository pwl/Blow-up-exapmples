#!/bin/bash

mplotx=
snapshot_dir="log/snapshot/"
logfile="log/info_1/log000.dat"
snapshot_name="*.dat"
mrows=5
mcols=5
mtot=$((mcols*mrows))
mspace=$((mtot))
# snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
#     sort -n -t '_' -k2 | awk 'NR > 1100' | awk 'NR % 5 == 0' | head -n $mtot  )
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t '_' -k2  | awk 'NR % 200 == 1' | head -n $mtot  )
startx=0.1
starty=0.1
stopx=0.9
stopy=0.9
xrange="50"
yrange="1.2*pi"
size_mult=1.
T=$(awk 'BEGIN {max=0} {if($2 > max) max=$2} END {printf("%.15E\n",max)}' $logfile)

echo "" > plotter.gp
echo "set terminal postscript enhanced color size 7,7" >> plotter.gp
echo "set output \"graphics/snapshot_to_multiplot.ps\"" >> plotter.gp
# echo "set size $size_mult,$size_mult" >> plotter.gp
echo "set multiplot title \"Blow-up profile (moving mesh method)\"" >> plotter.gp

i=0

for snap in $snapshot_files; do

    t=$(awk '/t = /{print $4}' $snap)
    g=$(awk '/g = /{print $4}' $snap)
    s=$(awk '/s = /{printf("%.0f",$4)}' $snap)
    du=$(awk '/du = /{printf("%.0f",$4)}' $snap)
    # T_t=$(echo "scale=20; $T-$t|bc")
    echo $t

    # echo "set logscale x 10" >> plotter.gp
    echo "set parametric" >> plotter.gp
    echo "set key off" >> plotter.gp
    echo "set xrange [ -2:2 ]; set yrange [ -2:2 ]; set trange [ 0:2*pi ]" >> plotter.gp
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

    # echo "set title \"t=$t\" offset screen -$sizex/$mcols, screen -($sizey+.1)/$mrows font \"Times-Roman,10\"" >> plotter.gp
    echo -ne "plot \"$snap\" u ((1.+.5*sin(\$1))*sin(\$2/sin(\$1))):((1.+.5*sin(\$1))*cos(\$2/sin(\$1))) t\"\" w l lw 2," >> plotter.gp
    echo -ne " sin(t),cos(t)\n" >> plotter.gp
    # echo -ne "plot \"$snap\" w l"

    # echo -ne "\"$blowup_file1\" t\"\" w l," >> plotter.gp
    # echo -ne "\"$blowup_file2\" u 1:(pi-\$2) t\"\" w l\n" >> plotter.gp
    # echo -ne "\"$blowup_file2\" u 1:((\$4)) index 1 w l lt 2 t\"\", 0 t\"\"\n" >> plotter.gp

    i=$((i+1))
done

echo "unset multiplot" >> plotter.gp

./snapshot_to_multiplot.gp
ps2pdf graphics/snapshot_to_multiplot.ps graphics/snapshot_to_multiplot.pdf
evince graphics/snapshot_to_multiplot.ps
