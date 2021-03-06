#!/bin/bash

info_file="log/info_1/log000.dat"
mplotx=
snapshot_dir="log/snapshot"
snapshot_name="*.dat"
mrows=3
mcols=3
mtot=$((mcols*mrows))
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t'_' -k2 | awk '(NR-1) % 240 == 0'| head -n$mtot)
blowup_file="harvester_data/shrinker_k3.00000_l1.0.dat"
startx=0.1
starty=0.1
stopx=0.9
stopy=0.9
xrange="50"
yrange="1.2*pi"

# echo "$snapshot_files"
# exit 0

echo "set multiplot title \"Convergence of u(y,s) to u_1(y), L=50\"" > plotter.gp


i=0

for snap in $snapshot_files; do

    t=$(./snap_to_time.sh $snap)
    t=${t:0:3}

    # setup the tics for picture in the lower left corner
    echo "unset tics" >> plotter.gp
    tics=$(echo "($i%$mrows==0) && (($i/$mcols)+1==$mrows)"|bc)
    if [ $tics -eq 1 ]; then
    	echo "set tics nomirror in" >> plotter.gp
	# echo "set xtics 0,$xrange/5,$xrange format \"\"" >> plotter.gp
	# echo "set xtics add (\"0\" 0, \"$xrange\" $xrange)" >> plotter.gp
	echo "set ytics 0,pi/4,pi format \"\"" >> plotter.gp
	echo "set ytics add (\"0\" 0, \"{/Symbol p}/2\" pi/2,  \"{/Symbol p}\" pi)" >> plotter.gp
    fi

    echo "set logscale x 10" >> plotter.gp
    echo "set key off" >> plotter.gp
    # switch margins off
    echo "set rmargin 0" >> plotter.gp
    echo "set lmargin 0" >> plotter.gp
    echo "set tmargin 0" >> plotter.gp
    echo "set bmargin 0" >> plotter.gp
    # setupt plot title
    # prod=$(./scalar_prod.sh $snap)
    # echo $t $prod
    echo "set title \"s=$t\" offset 5,-9 " >> plotter.gp
    # setup plot size and position
    sizex=$(echo "scale=20;($stopx-$startx)/$mrows"|bc)
    sizey=$(echo "scale=20;($stopy-$starty)/$mcols"|bc)
    echo "set size $sizex,$sizey" >> plotter.gp
    orx=$(echo "a=($i%$mrows);scale=20;$startx+a*($stopx-$startx)/$mrows"|bc)
    ory=$(echo "a=($i/$mcols);scale=20;$stopy-(($stopy-$starty)*(a+1))/$mcols"|bc)
    echo "set origin $orx,$ory" >> plotter.gp
    # plot the data
    echo -ne "plot [.01:$xrange] [0:1.2*pi] \"$snap\" u 1:(\$2/(50-\$1)) w l t\"\"," >> plotter.gp
    # echo -ne "plot [0:40] [0:pi] \"$snap\" u 1:2 w l t\"\"," >> plotter.gp
    echo -ne "\"$blowup_file\" index 0 w l t\"\"," >> plotter.gp
    echo "pi lt 3 t\"\"" >> plotter.gp

    i=$((i+1))
done

echo "unset multiplot" >> plotter.gp

./eigenvector_convergence_sy.gp
evince eigenvector_convergence_sy.ps
