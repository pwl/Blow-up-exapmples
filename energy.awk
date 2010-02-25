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
    f=$2
    fy=$3
    w=exp(-y*y/4)*y^(N-1)*(fy*fy/2 + L*(L+N-2)/2*sin(f)*sin(f)/y/y)
    printf "%.15G %.15G\n", y, w
    empty=0
}

END {
}
