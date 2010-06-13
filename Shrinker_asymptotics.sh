#!/bin/bash

shrinker_file=$1

blocks=$(grep -c 'A = ' $1)
asymptotics="Shrinker_asymptotics.dat"
exp_asymptotics="Expander_asymptotics.dat"
tempfile="Shrinker_asymptotics.tmp"
grid=""
plot="plot"

echo "" > test.dat

rm -f $asymptotics
echo "blocks = $blocks"

for i in $(seq 1 $blocks); do
    echo "Extracting block $i/$blocks"
    awk -v "block=$i" -f extract_block.awk $shrinker_file > $tempfile
    echo "Fitting data"
    # echo -ne "$i " >> $asymptotics
    b=$(./Shrinker_asymptotics_fit.gp)
    a=$(grep 'A' $tempfile |awk '{print $4}')
    En=$(grep 'En' $tempfile |awk '{print $4}')
    echo "$i   $a   $b   $En" >> $asymptotics
    if [ 1 -eq $(echo "$i%2"|bc) ]; then
    	bi="b_$i"
    	lt="1"
    else
    	bi="-b_$i"
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

# ./Shrinker_asymptotics.gp

rm $tempfile

# ./an_bn_theoretical.awk Shrinker_asymptotics.dat > an_bn_theoretical.dat
# ./an_bn_theoretical.awk Shrinker_asymptotics.dat| ./table_to_latex.awk > an_bn_theoretical.tex
