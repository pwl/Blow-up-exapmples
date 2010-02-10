#!/bin/bash

DIR=harvester_data
LIST_FILE=$(mktemp)
TEMP_FILE=$(mktemp)
PLOTTER_FILE="plotter.gp"
CMD="plot"

cd $DIR

touch $LIST_FILE

for file in eigen_k*.dat; do
    echo "$file"|sed 's|.*l\([.0-9][.0-9]*\).*|\1|' >> $LIST_FILE
done

for l in $(cat $LIST_FILE | sort | uniq); do
    cat eigen_k*l$l* > eigen_l$l.dat
    lw=$(echo "scale=0; (0==($l % .5))+(0==($l % 1))+1"|bc)
    lt=$(echo "4-$lw"|bc)
    CMD="$CMD \"$DIR/eigen_l$l.dat\" u 1:6 w l lt $lt lw $lw t \"\","
done

CMD=${CMD%,*}

cd ..

echo "$CMD" > $PLOTTER_FILE

./eigenvalue_plot.gp

evince eigenvalue_plot.ps&
