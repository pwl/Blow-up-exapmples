#!/bin/bash

DIR=harvester_data
PLOTTER_FILE="plotter.gp"
K=3.00000
L=2.0
INDEX=5


for L in $(seq 1 2); do
    for K in $(seq 3 4); do
	CMD="plot"

	for I in $(seq 1 $INDEX); do
	    file="shrinker_k${K}.00000_l${L}.0.dat"
	    CMD="$CMD \"$DIR/$file\" u ((-1)**($I+1)*(\$2-pi/2)):(-\$1*\$3*(-1)**($I+1)) index ($I-1) w l lw 2 t \"\","
	done

	CMD=${CMD%,*}
# cd ..

	echo "" > $PLOTTER_FILE
	echo "set output \"shrinker_profile_phase_N${K}_L${L}.ps\";" >> $PLOTTER_FILE
	echo "set title \"Profiles of shrinkers for N=$K L=$L\\nwith index from 1 to $INDEX\";" >> $PLOTTER_FILE
	echo -e "$CMD" >> $PLOTTER_FILE
	./shrinker_profiles_phase.gp
    done
done


# for file_ps in shrinker_profile*_i${INDEX}.ps; do
#     evince $file_ps &
# done
