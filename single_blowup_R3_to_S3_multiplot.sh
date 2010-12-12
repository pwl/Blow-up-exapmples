#!/bin/bash

mplotx=
snapshot_dir="log/snapshot/"
logfile="log/info_1/log000.dat"
snapshot_name="*.dat"
mrows=3
mcols=3
mtot=$((mcols*mrows))
mspace=$((mtot))
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t '_' -k2 | awk 'NR > 2100' | awk 'NR % 10 == 0' | head -n $mtot  )
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t '_' -k2  | awk 'NR % 200 == 0' | head -n $mtot  )
blowup_file1="harvester_data_shrinker/shrinker_reversed_k3.00000_l1.0.dat"
blowup_file2="harvester_data_expander/expander_k3.00000_l1.0.dat"
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
# tempfile2="temp2.dat"
tempfile3=$(tempfile)
# tempfile3="temp3.dat"

# select the first possible shrinker and
./extract_block.awk -v block=1 $blowup_file1 | awk '/^[0-9]/ {print}' > $tempfile1

# echo "$snapshot_files"
# # less $tempfile1
# exit 0

cat << EOF > plotter.gp
set terminal postscript enhanced color size 7,7
set output "graphics/single_blowup_R3_S3.ps"
set label "Single blow-up" at screen 0.3, 0.95 font "Times-Roman,10"
set multiplot title ""
EOF


i=0

for snap in $snapshot_files; do

    tempfile2=$(tempfile)
    tempfile3=$(tempfile)

    t=$(awk '/t = /{print $4}' $snap)
    g=$(awk '/g = /{print $4}' $snap)
    gprint=$(awk '/g = /{printf("%1.2E\n",$4)}' $snap)
    s=$(awk '/s = /{printf("%.0f",$4)}' $snap)
    du=$(awk '/du = /{printf("%.0f",$4)}' $snap)
    # T_t=$(echo "scale=20; $T-$t|bc"|awk '{printf("%1.2E",$1)}')

    echo "set logscale x 10" >> plotter.gp
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
	echo "set ylabel \"u(|T-t|^{1/2}y,t)\" rotate offset screen .1*$sizex, 0." >> plotter.gp
    fi

    echo "set title \"T-t=$gprint\" offset screen -.05*$sizex, screen -.80*$sizey font \"Times-Roman,10\"" >> plotter.gp
    # echo "set title \"t=$t\" offset screen -$sizex/$mcols, screen -($sizey+.1)/$mrows font \"Times-Roman,10\"" >> plotter.gp
    echo -ne "plot [1e-2,1e4] \"$snap\" u (\$1/sqrt($g)):(\$2/(\$1)) t\"\" w l," >> plotter.gp
    echo -ne "\"$blowup_file1\" t\"\" w l," >> plotter.gp
    echo -ne "\"$blowup_file2\" u 1:(pi-\$2) t\"\" w l\n" >> plotter.gp
    # echo -ne "\"$blowup_file2\" u 1:((\$4)) index 1 w l lt 2 t\"\", 0 t\"\"\n" >> plotter.gp

    i=$((i+1))
done

echo "unset multiplot" >> plotter.gp

chmod a+x plotter.gp
gnuplot plotter.gp

ps2pdf graphics/single_blowup_R3_S3.ps graphics/single_blowup_R3_S3.pdf
evince graphics/single_blowup_R3_S3.ps
