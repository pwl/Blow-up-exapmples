#!/bin/bash

# options="u 1:(\$1 < 0.3 ? \$8 : 1/0) w l notitle"
options1="w l lt 1 " # lt 1 pt 1"
options2="w p lt 2 pt 2"

name="*.dat"
path="log/snapshot/"
# files=$(find $path -name "$name" |sort| head -n 10)
files=$(find $path -name "$name" |sort -t'_' -k2n| awk 'NR % 20 == 0')
DT=10.
T_LAST=0.

mkdir -p movie
rm -f movie/*.png

for f in $files; do
    t=$(awk '/t = /{print $4}' $f)
    g=$(awk '/g = /{print $4}' $f)
    s=$(awk '/s = /{printf("%.0f",$4)}' $f)
    timer=$s

    TRIGGER=$(echo "scale=20;$timer>$T_LAST"|bc)
    # TRIGGER="1"
    # echo $T_LAST $DT $t
    # echo "scale=20;$t>$T_LAST"|bc
    if [ $TRIGGER = 1 ]; then
	T_LAST=$(echo "scale=20;$T_LAST+$DT"|bc)
	cmd="plot"			# initialize cmd
	echo "" > plotter.gp	# clear the temporary file
	output="movie/m_$timer.png"	# output file name
	title="Blow-up for harmonic maps heat flow\\nt=$t" # graph title
	cmd="set title \"$title\"; $cmd"	      # set the title
	cmd="set output \"$output\"; $cmd" # set the output file
        # cmd=" $cmd" # set the output file
        # cmd="$cmd \"$f\" u (log(tan(\$1/2.))):(\$2/sin(\$1)) every ::1::$nmb w lp" # the plot
	# cmd="$cmd \"$f\" u ((1.+.3*sin(\$1))*sin(\$2)):((1.+.3*sin(\$1))*cos(\$2)) t\"\" w l lw 2, sin(t),cos(t)"
	cmd="$cmd \"$f\" u ((1.+\$1/pi)*sin(\$2)):((1.+\$1/pi)*cos(\$2)) t\"\" w l lw 2, sin(t),cos(t)"
	# cmd="$cmd \"$f\" u 1:(\$2/\$1) w lp" # the plot
	echo "t = $t"		 # some output for user
	echo "$cmd" > plotter.gp # echo the command to the temporary file
	./snapshot_to_png_template.gp # execute the template (it loads the temporary file)
    fi
done

./make_movie.sh
# rm movie/*.png
mplayer movie.avi
