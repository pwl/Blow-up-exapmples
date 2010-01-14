#!/bin/bash

# options="u 1:(\$1 < 0.3 ? \$8 : 1/0) w l notitle"
options1="w l lt 1 " # lt 1 pt 1"
options2="w p lt 2 pt 2"

name="*.dat"
path="log/snapshot/"
# files=$(find $path -name "$name" |sort| head -n 10)
files=$(find $path -name "$name" |sort| awk 'NR % 1 == 0')
DT=.001
T_LAST=0.

mkdir -p movie
rm -f movie/*.png

for f in $files; do
    t=${f:18:17}		# determine time from file name

    TRIGGER=$(echo "scale=20;$t>$T_LAST"|bc)
    TRIGGER="1"
    if [ $TRIGGER = "1" ]; then
	T_LAST=$(echo "scale=20;$T_LAST+$DT"|bc)
	cmd="plot"			# initialize cmd
	echo "" > plotter.gp	# clear the temporary file
	output="movie/m_$t.png"	# output file name
	title="bubbling off for harmonic maps\\nt=$t" # graph title
	cmd="set title \"$title\"; $cmd"	      # set the title
	cmd="set output \"$output\"; $cmd" # set the output file
        # cmd="$cmd \"$f\" u (log(tan(\$1/2.))):(\$2/sin(\$1)) every ::1::$nmb w lp" # the plot
	cmd="$cmd \"$f\" u 1:2 w lp" # the plot
	echo "t = $t"		 # some output for user
	echo "$cmd" > plotter.gp # echo the command to the temporary file
	./snapshot_to_png_template.gp # execute the template (it loads the temporary file)
    fi
done

./make_movie.sh
# rm movie/*.png
gmplayer movie.avi
