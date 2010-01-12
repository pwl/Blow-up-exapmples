#!/bin/bash

# options="u 1:(\$1 < 0.3 ? \$8 : 1/0) w l notitle"
options1="w l lt 1 " # lt 1 pt 1"
options2="w p lt 2 pt 2"

name="*.dat"
path="log/snapshot2/"
files=$(find $path -name "$name" |sort| awk 'NR % 800 == 0')
DT=.05
T_LAST=0.
Di=3.1415
i=0.

mkdir -p movie
rm -f movie/*.png

cmd="plot"			# initialize cmd

for f in $files; do
    t=${f:19:17}		# determine time from file name

    TRIGGER=$(echo "scale=20;$t>$T_LAST"|bc)
    TRIGGER="1"
    if [ $TRIGGER = "1" ]; then
	T_LAST=$(echo "scale=20;$T_LAST+$DT"|bc)
	nmb=$(grep -v \# $f| wc -l)	# determine the number of lines
	nmb=$((nmb-2))		# before the last data point
	# cmd="$cmd \"$f\" u (log(tan(\$1/2.))):(\$2/sin(\$1)) every ::1::$nmb w lp" # the plot
	cmd="$cmd \"$f\" u (log(tan(\$1/2.))):(\$2/sin(\$1)+$i) every ::1::$nmb w l lw 1 lt 1," # the plot
	echo "t = $t"		 # some output for user
	i=$(echo "scale=20;$i+$Di"|bc)
    fi
done

cmd=${cmd%,*}

echo "$cmd" > plotter.gp # echo the command to the temporary file
./snapshot_to_png_template.gp # execute the template (it loads the temporary file)
