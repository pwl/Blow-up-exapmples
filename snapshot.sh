#!/bin/bash

# options="u 1:(\$1 < 0.3 ? \$8 : 1/0) w l notitle"
options1="w lp lt 1 " # lt 1 pt 1"
options2="w p lt 2 pt 2"

name="*.dat"
path="log/snapshot/"
files=$(find $path -name "$name" |sort -t'_' -k2n |tail -n 2000 | awk 'NR % 10 == 0')
cmd="plot"
g="bisection.dat"

./fitT.gp
T=$(cat T.dat)

echo $T

echo "" > plotter.gp

for f in $files; do
    t=${f:18:17}
    echo "t   = $t"
    tt=$(echo "scale=20;$T-$t"|bc)
    echo "T-t = $tt"
    # cmd="$cmd \"$f\" u (\$1/lambda($tt) < 10 ? \$1/lambda($tt) : 1/0):2 $options1 t \"t\'=$tt\","
    cmd="$cmd \"$f\" $options1 t \"t\'=$tt\","

    # cmd="$cmd \"$g\" u (\$1 < 10 ? \$1 : 1/0):2 $options2,"
    # cmd="$cmd \"$g\" u (\$1 < 10 ? \$1*sqrt($T-$t) : 1/0):2 $options2,"
done

# cmd="$cmd \"$g\" $options2 t \"Blowup profile\","

cmd=${cmd%,*}

echo -e "$cmd" >> plotter.gp

# echo -ne ", \"test.dat\" u 1:(\$1<6 ? \$2 : 1/0) w l linetype 1 lw 3\n" >> plotter.gp

# chmod a+x plotter.gp
# ./plotter.gp
./template.gp

# ps2pdf blowup.ps
