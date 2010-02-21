#!/bin/bash

info_file="log/info_1/log000.dat"
snapshot_dir="log/snapshot"
snapshot_name="*.dat"
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort | awk 'NR % 3 == 0')

tempfile=$(tempfile)

file_tointerpolate="harvester_data/shrinker_k3.00000_l1.0.dat"
file_interpolated="splined.dat"
spline="spline -t 0.0 8.0 .001 -P 15"

cat $file_tointerpolate | awk '/^$/{exit 1};{print $0}' |$spline -d2 \
 > $file_interpolated

# ./fitT.gp
T=$(cat T.dat)

cmd="plot "

i="0"
for f in $snapshot_files; do
    du=$(cat $f | sed -n -e 's|.*du = \(.*\)|\1|p')
    g=$(cat $f | sed -n -e 's|.*g = \(.*\)|\1|p')
    t=$(cat $f | sed -n -e 's|.*t = \(.*\)|\1|p')
    s=$(cat $f | sed -n -e 's|.*s = \(.*\)|\1|p')
    # echo "scale=20; $T-$t"|bc
    # N=$(cat $f | sed -n -e 's|.*N = \(.*\)|\1|p')
    lambda=$(awk -v e="$du" 'BEGIN {print 1./e}' )
    m=$(awk -v g="$T" -v l="$t" 'BEGIN {print 1./sqrt(g-l)}')
    k=$(awk -v g="$T" -v l="$t" 'BEGIN {print sqrt(g-l)}')

    lw=$(echo "(0==($i % 5))+(0==($i % 10))+1"|bc)
    lt=$(echo "4-$lw"|bc)
    i=$((i+1))

    if [ $(echo "$s >  700" | bc) = "1" ]; then
	break;
    fi
    echo "$i $s"

    cmd="$cmd \"< $PWD/eigenvector_convergence_helper.sh $f $m\" "
    cmd="$cmd u 1:(abs(\$2)) w l lt $lt lw $lw t \"\","
    # cmd="$cmd set label \"$k\" at "
done

cmd="$cmd \"eigen1.dat\" u 1:(abs(\$4)*100) w l lt 5 lw 3 t \"Normalized profile from ODE\"" > plotter.gp

# cmd=${cmd%,*}

output="eigenvector_convergence_N3.ps"

echo "set title \"Second eigenmode for N=3\\nODE vs PDE comparison\"" >> plotter.gp
echo "set output \"$output\"" >> plotter.gp
echo "$cmd" >> plotter.gp

./eigenvector_convergence.gp
evince $output
