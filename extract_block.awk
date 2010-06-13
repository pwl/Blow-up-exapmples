#!/usr/bin/awk -f

BEGIN {
    b=1;
    empty=0;
    # print L, " ", N
}

# ignore comment lines and empty lines
# /#/ {
#     next
# }

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
    print
}

b>block {
    exit
}

END {
}
