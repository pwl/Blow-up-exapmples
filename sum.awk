BEGIN {
    sum=0.;
}

{				# ignore comment lines
    sum+=$2
}

END {
    printf "%.15G", sum;
}
