#!/bin/bash

files=$(find . -name "bisection*.dat" | sort)
cmd="plot"
options="u (log(\$1)):2 w l"
n=0

echo "" > bisection.gp

for f in $files; do
    # let n++
    t=$(echo "$f"| sed -n 's|\./bisection\([0-9.]*\)\.dat|\1|p')
    cmd="$cmd \"$f\" $options t \"f'(x=0)=$t\","
done

cmd="$cmd 3.1415/2. t \"pi\","

cmd=${cmd%,*}

echo -e "$cmd" >> bisection.gp

./bisection_template.gp
