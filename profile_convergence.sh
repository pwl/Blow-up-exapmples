#!/bin/bash

info_file="log/info_1/log000.dat"
snapshot_dir="log/snapshot"
snapshot_name="*.dat"
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort| awk 'NR % 2 == 0')

cmd="plot "

i="0"
for f in $snapshot_files; do
    # spatial derivative at origin (u_x(0,t))
    du=$(cat $f | sed -n -e 's|.*du = \(.*\)|\1|p')
    # dimension
    N=$(cat $f | sed -n -e 's|.*N = \(.*\)|\1|p')
    lambda=$(awk -v e="$du" 'BEGIN {print 1/e}' )
    # line width and line type directives
    lw=$(echo "(0==($i % 5))+(0==($i % 10))"|bc)
    lt=$(echo "3-$lw"|bc)
    cmd="$cmd \"$f\" u (\$1/$lambda):2 w l lt $lt lw $lw t \"\","
    i=$((i+1))
done

# finish the command by ploting the apropriate static solution
cmd="$cmd \"static_$N.dat\" w l lt 5 lw 1 t \"Static solution\"" >> plotter.gp

# output to ps file
output="profile_convergence_N$N.ps"

echo "set title \"Convergence of blow-up profile for N=$N\"" > plotter.gp
echo "set output \"$output\"" >> plotter.gp
echo "$cmd" >> plotter.gp

./profile_convergence.gp
evince $output
