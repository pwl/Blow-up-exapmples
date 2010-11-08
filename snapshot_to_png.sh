#!/bin/bash

# options="u 1:(\$1 < 0.3 ? \$8 : 1/0) w l notitle"
options1="w l lt 1 " # lt 1 pt 1"
options2="w p lt 2 pt 2"

name="*.dat"
path="log/snapshot/"
# files=$(find $path -name "$name" |sort| head -n 10)
files=$(find $path -name "$name" |sort -t'_' -k2n| awk 'NR % 1 == 0')
DT=.001
T_LAST=0.

mkdir -p movie
rm -f movie/*

TRIGGER="1"
i=1;


for f in $files; do
    # t=$(awk '/t = /{print $4; exit}' $f)
    t=$(awk '/t = /{printf("%.3f",$4); exit}' $f)
    g=$(awk '/g = /{print $4; exit}' $f)
    s=$(awk '/s = /{printf("%.0f",$4); exit}' $f)
    timer=$t

    TRIGGER=$(echo "scale=20;$timer>=$T_LAST"|bc)
    # echo $T_LAST $DT $t
    # echo "scale=20;$t>$T_LAST"|bc
    if [ $TRIGGER = 1 ]; then
    	# TRIGGER=0
    	T_LAST=$(echo "scale=20;$T_LAST+$DT"|bc)
	T_LAST=$(echo "$T_LAST $timer"|awk '$1 >= $2 {print $1} $2 > $1 {print $2}')
	cmd="plot"			# initialize cmd
    	echo "" > plotter.gp	# clear the temporary file
    	output=$(printf "movie/m_%010i.svg\n" $i)	# output file name
    	i=$((i+1))
    	# echo "$output"
    	title="Blow-up for harmonic maps heat flow\\nt=$t" # graph title
    	cmd="set title \"$title\"; $cmd"	      # set the title
    	cmd="set output \"$output\"; $cmd" # set the output file
        # cmd=" $cmd" # set the output file
        # cmd="$cmd \"$f\" u (log(tan(\$1/2.))):(\$2/sin(\$1)) every ::1::$nmb w lp" # the plot
    	# cmd="$cmd \"$f\" u ((1.+.3*sin(\$1))*sin(\$2)):((1.+.3*sin(\$1))*cos(\$2)) t\"\" w l lw 2, sin(t),cos(t)"
    	cmd="$cmd \"$f\" u ((1.+.5*sin(\$1))*sin(\$2)):((1.+.5*sin(\$1))*cos(\$2)) t\"\" w l lw 3, sin(t),cos(t)"
    	# cmd="$cmd \"$f\" u 1:(\$2/\$1) w lp" # the plot
    	echo "t = $t"		 # some output for user
    	echo "$cmd" > plotter.gp # echo the command to the temporary file
    	./snapshot_to_png_template.gp # execute the template (it loads the temporary file)
    	rsvg -f png $output ${output%%.svg}.png
    	rm $output
    fi
done

./make_movie.sh
# rm movie/*.png
mplayer movie.avi -loop 0
