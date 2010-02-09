#!/bin/bash

DIR=harvester_data
PLOTTER_FILE="plotter.gp"
INDEX=$(echo "$1+1"|bc)

for K in $(seq 3 6); do
    cd $DIR
    CMD="plot"
    for file in shrinker_k$K.0*.dat; do
	l=$(echo "$file"|sed 's|.*l\([0-9]\.[0-9]\).*|\1|')
	lw=$(echo "scale=0; (0==($l % .5))+(0==($l % 1))+1"|bc)
	lt=$(echo "4-$lw"|bc)
	# echo "$K $l $file"
	CMD="$CMD \"$DIR/$file\" u 1:2 index $1 w l lt $lt lw $lw t \"\","
    done
    CMD=${CMD%,*}
    cd ..
    echo "set output \"shrinker_profile_N${K}_i${INDEX}.ps\"" > $PLOTTER_FILE
    echo "set title \"Profiles of shrinkers for N=$K, index=$INDEX\\nL varying from 1.0 (leftmost) to 3.0\"" >> $PLOTTER_FILE
    echo "$CMD" >> $PLOTTER_FILE
    ./shrinker_profiles.gp
done


# for file_ps in shrinker_profile*_i${INDEX}.ps; do
#     evince $file_ps &
# done
