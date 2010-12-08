#!/bin/bash

read -d '' files
DT=$1
T_LAST=0.



for f in $files; do
    # read the time
    t=$(awk '/t = /{printf("%.20f",$4); exit}' $f)

    TRIGGER=$(echo "scale=20;$t>=$T_LAST"|bc)
    if [ $TRIGGER = 1 ]; then
    	# T_LAST+=DT
    	T_LAST=$(echo "scale=20;$T_LAST+$DT"|bc)
    	# T_LAST=max(T_LAST,timer)
    	T_LAST=$(echo "$T_LAST $t"|awk '$1 >= $2 {print $1} $2 > $1 {print $2}')
    	echo $f
    fi
done
