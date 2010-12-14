#!/bin/bash

mplotx=
snapshot_dir="log/snapshot_single_blow-up_R3_to_R3/"
logfile="log/info_1/log000_single_blow-up_R3_to_R3.dat"
snapshot_name="*.dat"
mrows=3
mcols=3
mtot=$((mcols*mrows))
mspace=$((mtot))
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t '_' -k2 | awk 'NR > 2100' | awk 'NR % 10 == 0' | head -n $mtot  )
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t '_' -k2  | awk 'NR % 350 == 1 && NR > 1071' | head -n $mtot  )
snapshot_files=$(find $snapshot_dir/choice -name "$snapshot_name" |
    sort -n -t '_' -k2 | head -n $mtot )
blowup_file1="harvester_data_shrinker/shrinker_reversed_k3.00000_l1.0.dat"
blowup_file1="harvester_data_shrinker/shrinker_k3.00000_l1.0.dat"
blowup_file2="harvester_data_expander/expander_k3.00000_l1.0.dat"
startx=0.1
starty=0.1
stopx=0.9
stopy=0.9
xrange="50"
yrange="1.2*pi"
size_mult=1.

T="0.01574053748"

# cp $snapshot_files $snapshot_dir/choice


# select the first possible shrinker and
# echo "$snapshot_files"
# # less $tempfile1
# exit 0

cat << EOF > plotter.gp
set terminal postscript enhanced size 7,7
set output "graphics/single_blowup_R3_S3.ps"
# set label "Single blow-up" at screen 0.3, 0.95 font "Times-Roman,10"
set multiplot title ""
EOF


i=0

for snap in $snapshot_files; do

    t=$(awk '/t = /{printf("%.15f",$4)}' $snap)
    g=$(awk '/g = /{print $4}' $snap)
    gprint=$(awk '/g = /{printf("%1.2E\n",$4)}' $snap)
    s=$(awk '/s = /{printf("%.0f",$4)}' $snap)
    du=$(awk '/du = /{printf("%.0f",$4)}' $snap)
    T_t=$(echo "scale=20; $t-$T" |bc |sed -e 's|[^-]||g')
    # echo "$i $T_t"

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
	echo 'set ytics (0,  "{/Symbol p}" pi)' >> plotter.gp
	echo 'set xtics (1e-2, 1,  1e2, 1e4)' >> plotter.gp
	echo "set xlabel \"y\"" >> plotter.gp
	echo "set ylabel \"u(t,(T-t)^{1/2}y)\" rotate offset screen .05*$sizex, 0." >> plotter.gp
	echo 'set format x "10^{%L}"' >> plotter.gp

    fi

    echo "set title \"t-T = $T_t$gprint\" offset screen .15*$sizex, screen -1.0*$sizey font \"Times-Roman,10\"" >> plotter.gp
    # echo "set title \"t=$t\" offset screen -$sizex/$mcols, screen -($sizey+.1)/$mrows font \"Times-Roman,10\"" >> plotter.gp
    echo -ne "plot [1e-2:1e6] [0:1.2*pi] \"$snap\" u (\$1/sqrt($g)):(\$2/(\$1)) t\"\" w l lw 2," >> plotter.gp
    # echo -ne "\"$blowup_file1\" t\"\" w l," >> plotter.gp
    echo -ne "\"$blowup_file1\" u 1:(\$1<10?\$2:1/0) index 0 w l lt 2 lw 1, pi w l lt 2 lw 1, x>10?pi/2+0.573141133043885:1/0 w l lt 2 lw 1, pi w l lt 3 lw 1," >> plotter.gp
    echo -ne "\"$blowup_file2\" u 1:(pi-\$2) t\"\" w l lt 2 lw 1\n" >> plotter.gp
    # echo -ne "\"$blowup_file2\" u 1:((\$4)) index 1 w l lt 2 t\"\", 0 t\"\"\n" >> plotter.gp

    i=$((i+1))
done

echo "unset multiplot" >> plotter.gp

chmod a+x plotter.gp
gnuplot plotter.gp

ps2pdf graphics/single_blowup_R3_S3.ps graphics/single_blowup_R3_S3.pdf
evince graphics/single_blowup_R3_S3.ps
