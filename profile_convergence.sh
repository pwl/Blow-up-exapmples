#!/bin/bash

info_file="log/info_1/log000.dat"
snapshot_dir="log/snapshot"
snapshot_name="*.dat"
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort| awk 'NR % 50 == 0')

cmd="plot "

i="0"
for f in $snapshot_files; do
    du=$(cat $f | sed -n -e 's|.*du = \(.*\)|\1|p')
    N=$(cat $f | sed -n -e 's|.*N = \(.*\)|\1|p')
    lambda=$(awk -v e="$du" 'BEGIN {print 1/e}' )
    lw=$(echo "(0==($i % 5))+(0==($i % 10))+1"|bc)
    lt=$(echo "4-$lw"|bc)
    cmd="$cmd \"$f\" u (\$1/$lambda):2 w l lt $lt lw $lw t \"\","
    i=$((i+1))
done

cmd="$cmd \"static_$N.dat\" w l lt 5 lw 6 t \"Static solution\"" >> plotter.gp

echo "set title \"Convergence of blow-up profile for N=$N\"" > plotter.gp
echo "set output \"profile_convergence_N$N.ps\"" >> plotter.gp
echo "$cmd" >> plotter.gp

./profile_convergence.gp
