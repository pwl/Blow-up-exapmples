#!/bin/bash

DIR=harvester_data
PLOTTER_FILE="plotter.gp"
info_file="log/info_1/log000.dat"
snapshot_dir="log/snapshot"
snapshot_name="*.dat"
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort| awk 'NR % 5 == 0' )
    # | awk 'NR % 5 == 0')

file=$(echo "$snapshot_files" | head -n1)
l=$(grep -v '#' $file | wc -l)
evr=2
range=$(seq 2 $evr $l )

echo "Printing point positions to temporary files"
for i in $range; do
    temp[i]=$(mktemp)
    for f in $snapshot_files; do
	t=$(cat $f | sed -n -e 's|.*t = \(.*\)|\1|p')
	s=$(cat $f | sed -n -e 's|.*s = \(.*\)|\1|p')

	grep -v '#' $f | awk "NR==$i {print \$1, $s}" >> ${temp[$i]}
    done
done

# exit 0

echo "plotting"
cmd="plot"
i="1"
for temp_file in ${temp[*]}; do
    lw=$(echo "(0==($i % 5))+(0==($i % 10))+1"|bc)
    lt=$(echo "4-$lw"|bc)
    i=$((i+1))

    cmd="$cmd \"$temp_file\" w l lt 1 lw 1 t \"\", "
done
cmd=${cmd%,*}

echo "" > plotter.gp
echo "$cmd" >> plotter.gp
./moving_mesh_plot.gp

for f in ${temp[*]}; do
    rm $f
done
