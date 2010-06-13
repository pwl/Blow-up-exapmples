#!/usr/bin/awk -f

function abs(x){return x<0 ? -x : x}
function dec(x){return int(log(abs(x))/log(10))}
function norm(x){return x/10^dec(x)}
function printdec(d,n){
    if ( d==0 )
	printf("$\\,%.6f\\,$", n)
    else
	printf("$\\,%.6f \\cdot 10^{%i}\\,$", n, d)
}


BEGIN {
    start=1
    fields="$n$ & $a_n$ & $b_n$ & $E_n$\\\\\\hline\n"
    lines_max=10
}

start == 1 {
    printf("\\begin{tabular}{")
    for(i=1;i<=NF;i++)printf("|c")
    printf("|}\\hline\n")
    printf fields
    start = 0
}

NR <= lines_max {
    for ( i=1; i<NF; i++ )
    {
	d=dec($i)
	n=norm($i)

	if( int($i)==$i )	# integer!
	    printf("$\\,%i\\,$ & ",$i)
	else			# real!
	{
	    printdec(d,n)
	    printf(" & ")
	}
    }
    d=dec($NF)
    n=norm($NF)
    printdec(d,n)

    printf("\\\\\\hline \n", n, d)
}

END {
    printf("\\end{tabular}\n")
}
