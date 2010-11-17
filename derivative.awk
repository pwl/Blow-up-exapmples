#!/usr/bin/awk -f

# Simple script to generate a first order two-point right derivatives
# from data.
#
# The data produced is one line shorter than the input data

BEGIN { firstline = 1 }

$0 !~ /^#/ {
    if (firstline == 1) {
	old_x = $1
	old_y = $2
	firstline = 0
    }
    else {
	printf("%.20G %.20G\n", old_x , (old_y - $2)/(old_x - $1))
	old_x = $1
	old_y = $2
    }
}
