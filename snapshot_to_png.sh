#!/bin/bash

# options="u 1:(\$1 < 0.3 ? \$8 : 1/0) w l notitle"
options1="w l lt 1 " # lt 1 pt 1"
options2="w p lt 2 pt 2"

name="*.dat"
path="log/snapshot2/"
files=$(find $path -name "$name" |sort| awk 'NR % 10 == 0')

rm movie/*.png

for f in $files; do
    echo "" > plotter.gp	# clear the temporary file
    nmb=$(grep -v \# $f| wc -l)	# determine the number of lines
    nmb=$((nmb-2))		# before the last data point
    cmd="plot"			# initialize cmd
    t=${f:19:17}		# determine time from file name
    echo "t = $t"		# some output for user
    output="movie/m_$t.png"	# output file name
    title="bubbling off for harmonic maps\\nt=$t" # graph title
    cmd="set title \"$title\"; $cmd"		  # set the title
    cmd="set output \"$output\"; $cmd"		  # set the output file
    cmd="$cmd \"$f\" u (log(tan(\$1/2.))):(\$2/sin(\$1)) every ::1::$nmb w lp" # the plot
    echo "$cmd" > plotter.gp	# echo the command to the temporary file
    ./snapshot_to_png_template.gp # execute the template (it loads the temporary file)
done

./make_movie.sh
rm movie/*.png
gmplayer movie.avi
