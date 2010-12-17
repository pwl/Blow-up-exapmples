#!/bin/bash

mplotx=
snapshot_dir="log/snapshot/"
logfile="log/info_1/log000.dat"
snapshot_dir="log/snapshot/"
logfile="log/info_1/log000.dat"
snapshot_name="*.dat"
mrows=3
mcols=3
mtot=$((mcols*mrows))
mspace=$((mtot))
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t'_' -k2 | awk 'NR % 1646 == 1' | head -n$mtot|awk 'NR%1==0 {print}')
blowup_file1="harvester_data_shrinker/shrinker_k3.00000_l1.0.dat"
# blowup_file2="harvester_data_shrinker/eigen_k3.00000_l1.0_i1.dat"
blowup_file2="harvester_data_expander/expander_k3.00000_l1.0.dat"
startx=0.1
starty=0.1
stopx=0.9
stopy=0.9
size_mult=1.
T=$(awk 'BEGIN {max=0} {if($2 > max) max=$2} END {printf("%.20f",max)}' $logfile)
# T=0.328077505829169
# T=0.328077505829110
# T=0.328075426868397

cat << EOF > plotter.gp
set terminal postscript enhanced size 7,7
set output "graphics/Convergence_to_f1.ps"
# set label "Convergence to the blow-up profile" at screen 0.3, 0.95 font "Times-Roman,10"
set multiplot title ""
EOF

i=0

for snap in $snapshot_files; do

    t=$(awk '/t = /{printf("%.20f",$4)}' $snap)
    g=$(awk '/g = /{print $4}' $snap)
    gprint=$(awk '/g = /{printf("%1.2E\n",$4)}' $snap)
    s=$(awk '/s = /{printf("%.0f",$4)}' $snap)
    du=$(awk '/du = /{printf("%.0f",$4)}' $snap)
    T_t=$(echo "scale=20; $T-$t"|bc| awk '{printf("%.2E",$1)}')

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

    echo "set title \"T-t=$T_t\" offset screen .05*$sizex, screen -1.0*$sizey font \"Times-Roman,10\"" >> plotter.gp
    # echo -ne "plot [:pi] [ 0:pi ] \"$snap\" u (\$1/sqrt(abs($T-$t))):1 w l lw 2," >> plotter.gp
    echo -ne "plot [1e-2:1e4] [0:pi] \"$snap\" u (\$1/sqrt(abs($T-$t))):2 w l lw 2," >> plotter.gp
    echo -ne "\"$blowup_file1\" u 1:(\$1<10?\$2:1/0) index 0 w l lt 2 lw 1, pi w l lt 3 lw 1, x>10?pi/2+0.573141133043885:1/0 w l lt 2 lw 1 \n" >> plotter.gp

    i=$((i+1))
done

echo "unset multiplot" >> plotter.gp

chmod a+x plotter.gp
gnuplot plotter.gp
ps2pdf graphics/Convergence_to_f1.ps graphics/Convergence_to_f1.pdf
evince graphics/Convergence_to_f1.ps
