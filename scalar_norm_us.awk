#!/bin/awk -f

BEGIN {
    b=0;
    empty=0;
    # print L, " ", N
}

# ignore comment lines and empty lines
/#/ {
    next
}

# if line is empty increment block
/^$/ && empty == 0{
    empty=1
    b++
    next
}

!/^$/ {
    empty = 0
}

empty == 0 && b == block {
    y=$1
    if( y >= L )
	exit
    fy=($3*(L-y)+$2)/(L-y)/(L-y)
    fs=$4/(L-y)
    w=exp(-y*y/4)*y^(N-1)*(fs*fs)
    printf "%.15G %.15G\n", y, w
    empty=0
}

END {
}
