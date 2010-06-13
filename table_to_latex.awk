#!/usr/bin/awk -f

BEGIN {

}


NR>1{
    printf("$\\,%s\\,$ & $\\,%.6E\\,$ & $\\,%.6E\\,$ & \\\\\\hline\n", $1, $4, $5)
    # for ( i=1; i<NF; i++ )
    #     printf("$\\,%s\\,$ & ", $i)
    # printf("$\\,%s\\,$ \\\\\\hline \n", $NF)
}

END {

}
