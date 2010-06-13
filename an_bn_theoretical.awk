#!/usr/bin/awk -f

BEGIN {
    dim=3.
    c1=0.695220830587285
    omega=sqrt(8*dim-8-dim*dim)/2
    pi=3.14159265358979
    d1=-0.00928 #-0.00928848151332049
    c=-0.700472925187306
    d=1.80925718904119
    C=exp((d1-d)/omega)
    aratio=1/exp(pi/omega)
    bratio=-exp(pi/2./omega)
    while((getline  < "harvester_data/common_data" ) > 0 )
	annum[$3]=$4
    print "n    An/an             Bn/bn             an                bn                a(n+1)/an         b(n+1)/bn"
}

{
    if( NR > 1 ) {
	n=NR-1
	an=C*exp(n*pi/omega)
	bn=(-1)**n*c/c1*an**(-.5)
	printf "%2i  % .10E % .10E % .10E % .10E % .10E % .10E\n", n, an/anp, bn/bnp, anp, bnp,  anp/annum[n]/aratio, bnp/$2/bratio
    }
    anp=annum[NR-1]
    bnp=$2
}
