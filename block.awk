#!/usr/bin/awk -f

BEGIN {
    block=0;
    empty=1;
}

# ignore comment lines and empty lines
/#/ {
    next
}

# if line is empty increment block
/^$/ && empty == 0{
    empty=1
    next
}

!/^$/ && empty == 1 {
    block++
    empty=0
}

END {
    print block
}
