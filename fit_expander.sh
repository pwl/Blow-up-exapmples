#!/bin/bash

snapshot_dir="log/snapshot_single_blow-up_R3_to_S3/"
logfile="log/info_1/log000_single_blow-up_R3_to_S3.dat"
T=$(awk 'BEGIN {max=0} {if($2 > max) max=$2} END {print max}' $logfile)
# T=0.0342225289353554
snap1=$(find $snapshot_dir -name "*.dat" | sort -n -t'_' -k2 | awk 'NR == 5000')
snap2=$(find $snapshot_dir -name "*.dat" | sort -n -t'_' -k2 | awk 'NR == 8000')
mod_file="harvester_data_shrinker/eigen_k3.00000_l1.0_i1.dat"
T=0.034219246515

t1=$(awk '/t = /{printf("%.30f",$4)}' $snap1)
t2=$(awk '/t = /{printf("%.30f",$4)}' $snap2)

rm -f fit_two_lambda.dat time.dat plotter.gp

echo "T=$T+1e-16" >> plotter.gp
echo 'file="'$logfile'"' >> plotter.gp
echo 'mod_file="'$mod_file'"' >> plotter.gp
echo 'snap1="'$snap1'"' >> plotter.gp
echo 'snap2="'$snap2'"' >> plotter.gp
echo 't1='$t1 >> plotter.gp
echo 't2='$t2 >> plotter.gp

pyxplot fit_expander.gp
eps2eps graphics/fit_expander{,_opt}.eps
# rm graphics/plot.eps
