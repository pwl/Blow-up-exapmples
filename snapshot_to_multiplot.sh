#!/bin/bash

mplotx=
snapshot_dir="log/snapshot"
snapshot_name="*.dat"
mrows=5
mcols=5
mtot=$((mcols*mrows))
mspace=$((mtot))
# snapshot_N=$(find $snapshot_dir -name "$snapshot_name"|wc -l)
# snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
#     sort -n -t'_' -k2 | awk '(NR-1) % 1200 == 0'| head -n$mtot)
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t'_' -k2 | awk 'NR % 1 == 0 && |NR-13| > 1380'| head -n$mtot)
blowup_file1="harvester_data/shrinker_k3.00000_l1.0.dat"
blowup_file2="harvester_data/expander_k3.00000_l1.0.dat"
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
echo "set terminal postscript enhanced color size 7,7" >> plotter.gp
echo "set output \"graphics/snapshot_to_multiplot.ps\"" >> plotter.gp
# echo "set size $size_mult,$size_mult" >> plotter.gp
echo "set multiplot title \"Continuation beyond the blow-up\"" >> plotter.gp

i=0

for snap in $snapshot_files; do

    t=$(./snap_to_time.sh $snap)
    t=${t:0:3}

    g=$(awk '/g = /{print  $4}' $snap)
    s=$(awk '/s = /{printf("%.0f",$4)}' $snap)
    du=$(awk '/du = /{printf("%.0f",$4)}' $snap)

    # echo "set logscale x 10" >> plotter.gp
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
	echo 'set xtics (0,2,4)' >> plotter.gp

	# echo "set xtics (\"\\\$\\\pi/2\\\$\" pi/2)" >> plotter.gp
	echo 'set ytics ("b_1+{/Symbol p}/2" 0.573141+pi/2)' >> plotter.gp
	echo "set grid ytics" >> plotter.gp
    	# echo "set format x \"10^\{%L\}\"" >> plotter.gp
	# echo "set format x2 \"10^\{%L\}\"" >> plotter.gp
	# echo "set format y \"10^\{%L\}\"" >> plotter.gp
	# echo "set xtics add (\"\\\$0\\\$\" 0, \"\\\$\\\pi/4\\\$\" pi/4, \"\\\$\\\pi/2\\\$\" pi/2)" >> plotter.gp
	echo "set xlabel \"y\"" >> plotter.gp
	echo "set ylabel \"u(y,t)\" rotate offset screen .3*$sizex, -.2*$sizey" >> plotter.gp
	# echo "set ytics 0,pi/4,pi format \"\"" >> plotter.gp
	# echo "set ytics add (\"0\" 0, \"{/Symbol p}/2\" pi/2,  \"{/Symbol p}\" pi)" >> plotter.gp
    fi

    # if [ $i -eq $((mtot-1)) ]; then
    # 	echo "set label \"\\\$\\\vn\{1\}\{3\}\\\$\" at graph 0.3, graph 0.8" >> plotter.gp
    # 	echo "set label \"\\\$\\\vn\{3\}\{3\}\\\$\" at graph 0.1, graph 0.62" >> plotter.gp
    # fi

    # echo "set title \"s=$s\" offset screen -.2*$sizex, screen -.25*$sizey" >> plotter.gp
    echo "set title \"s=$s\" offset screen .2*$sizex, screen -.75*$sizey" >> plotter.gp
    # plot the data
    echo -ne "plot [0:10] [0:pi] \"$snap\" u (\$1/sqrt($g)):(\$2/\$1) t\"\" w lp ps .3," >> plotter.gp
    echo -ne "\"$blowup_file1\" index 0 w l lt 2 t\"\"," >> plotter.gp
    echo -ne "\"$blowup_file2\" u 1:(-\$2+pi/2) w l lt 2 t\"\"\n" >> plotter.gp
    # echo -ne "\"$blowup_file2\" u 1:(abs(\$4-\$1*\$3/2.738753125884604)/1.1e2) index 1 w l lt 2 t\"\"\n" >> plotter.gp
    # echo -ne "x w l lt 2 t\"\"\n" >> plotter.gp
    # echo -ne "\"$blowup_file2\" u 1:(abs(\$4)) w l lt 3 t\"\"\n" >> plotter.gp

    # echo "unset logscale xy" >> plotter.gp
    # # echo 'set logscale x2' >> plotter.gp
    # echo "unset xlabel; unset ylabel; unset tics; unset title; unset label" >> plotter.gp
    # orx=$(echo "scale=20;$orx+.6*$sizex"|bc)
    # ory=$(echo "scale=20;$ory+.0*$sizey"|bc)
    # echo "set origin $orx,$ory" >> plotter.gp
    # sizex=$(echo "scale=20;.4*$sizex"|bc)
    # sizey=$(echo "scale=20;.4*$sizey"|bc)
    # echo "set size $sizex,$sizey" >> plotter.gp

    # # tics=$(echo "($i%$mrows==0) && (($i/$mcols)+1==$mrows)"|bc)
    # if [ $i -eq 0 ]; then
    # 	echo "set tics nomirror in" >> plotter.gp
    # 	echo 'unset xtics; set x2tics (1.e-4, 1.e-1) format "10^\{%L\}"' >> plotter.gp
    # 	echo 'set ytics (0, "{/Symbol p}" pi)' >> plotter.gp
    # 	# echo "set xtics 0,pi/2,pi format \"\"" >> plotter.gp
    # 	# echo "unset xtics" >> plotter.gp
    # 	# echo "set x2tics (\"{/Symbol p}/2\" pi/2)" >> plotter.gp
    # 	# echo "set ytics (\"\\\$\\\pi/2\\\$\" pi/2)" >> plotter.gp
    # 	# echo "set grid x2tics" >> plotter.gp
    # fi

    # echo "plot [0:pi] [0:pi] \"$snap\" u 1:(\$2/\$1) w l lt 1" >> plotter.gp
    # # echo "\"\" u (pi-\$1):(pi-\$2) w l lt 1" >> plotter.gp

    i=$((i+1))
done

echo "unset multiplot" >> plotter.gp

./snapshot_to_multiplot.gp
# ps2pdf graphics/snapshot_to_multiplot.ps graphics/snapshot_to_multiplot.pdf
evince graphics/snapshot_to_multiplot.ps
