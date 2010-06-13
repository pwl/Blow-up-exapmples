#!/usr/bin/awk -f

BEGIN {
    c1=0.695220830587285
    omega=sqrt(7.)/2.
    pi=3.14159265358979
    d1=-0.00928848151332049
    c=-0.700472925187306
    d=1.80925718904119
    C=exp((d1-d)/omega)
}

{
    n=$1
    an=C*exp(n*pi/omega)
    bn=(-1)**n*c/c1*an**(-.5)
    printf "%.15E  %.15E %.15E\n", an, bn, $2
}
