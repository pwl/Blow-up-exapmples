#!/bin/bash

shrinker_file=$1
blocks=$(awk -f block.awk $shrinker_file)
asymptotics="Shrinker_asymptotics.dat"
exp_asymptotics="Expander_asymptotics.dat"
tempfile="Shrinker_asymptotics.tmp"
grid=""
plot="plot"

echo "" > test.dat

echo "" > $asymptotics
echo "blocks = $blocks"

for i in $(seq 1 $blocks); do
    echo "Extracting block $i/$blocks"
    awk -v "block=$i" -f extract_block.awk $shrinker_file > $tempfile
    echo "Fitting data"
    echo -ne "$i " >> $asymptotics
    a=$(./Shrinker_asymptotics_fit.gp)
    if [ 1 -eq $(echo "$i%2"|bc) ]; then
	bi="a_$i"
	lt="1"
    else
	bi="-a_$i"
	lt="2"
    fi
    grid="${grid}\"$bi\" abs($a-pi/2),"
    plot="$plot abs($a-pi/2) lt $lt t\"\","
done

grid=${grid%,*}
# plot=${plot%,*}

plot="$plot \"$exp_asymptotics\" u 1:(\$2-pi/2) w l lt 1 t \"Expander from x=0\",\
\"\" u 1:(pi/2-\$2) w l lt 2 t \"Expander from x={/Symbol p}\""

echo "" > plotter.gp
echo "set ytics ($grid)" >> plotter.gp
echo "set grid ytics noxtics"
echo "$plot" >> plotter.gp

./Shrinker_asymptotics.gp
