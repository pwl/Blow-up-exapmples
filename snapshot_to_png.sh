#!/bin/bash

# options="u 1:(\$1 < 0.3 ? \$8 : 1/0) w l notitle"
options1="w l lt 1 " # lt 1 pt 1"
options2="w p lt 2 pt 2"

name="*.dat"
path="log/snapshot_multiple_blow-up_S3_S3/"
# files=$(find $path -name "$name" |sort| head -n 10)
# files=$(find $path -name "$name" |sort -t'_' -k2n| awk 'NR % 20 == 0')
# files=$(find $path -name "$name" |sort -t'_' -k2n| ./select_by_time.sh 0.001)
files=$(cat choice2.dat)

# echo $files > choice2.dat
# exit 0

mkdir -p movie_{1,2}
for m in movie_{1,2}; do
    rm -f $m/*
done

TRIGGER="1"
i=1;

for f in $files; do

    t=$(awk '/t = /{printf("%.10f",$4); exit}' $f)
    echo "t=$t;i=$i;f=\"$f\"" > plotter.gp # data for pyxplot script
    echo "t = $t"		 # some output for user
    pyxplot snapshot_to_png_template.gp # execute the template (it
					# loads data through
					# plotter.gp)
    i=$((i+1))
done

for m in movie_{1,2}; do
    ./make_movie.sh $m
    # rm -f $m/*
done
# rm movie/*.png
# mplayer movie.avi -loop 0
