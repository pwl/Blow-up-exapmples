#!/bin/bash

snapshot_dir="log/snapshot/"
logfile="log/info_1/log000.dat"
snapshot_name="*.dat"
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t'_' -k2  | awk 'NR % 1 == 0 && NR >= 0' ) #|head -n20 )

T=0.328075426868397
T=0.328076268204451
T=0.328076268204451
# T=0.328076268204451

rm -f fit_two_lambda.dat
rm -f time.dat

for snap in $snapshot_files; do
    t=$(awk '/t = /{print $4}' $snap)
    Tt=$(awk -v "T=$T" '/t = /{printf("%.16G",T-$4);exit 0}' $snap)
    Tt=$(echo "scale=100; $T-$t"|bc)
    # Tt=$(echo "scale=20;$T-$t"|bc)

    # echo "$snap"
    # echo -ne "$(echo "scale=20; l(sqrt(if( $Tt > 0 ) $Tt else -$Tt))"|bc -l)\r"
    echo "$(echo "scale=20; a=$Tt; if( a < 0 ) a*=-1.; l(a)")"|bc -l
    echo "$t" >> time.dat

    # calculate xdu/dx+2(T-t)du/dt
    # first derivative
    # take value at x=0
    y=$(awk -v "Tt=$Tt" '/^[0-9].*/ && NR <=15 {printf("%.16G %.16G\n", $1/sqrt(Tt>0?Tt:-Tt), ($1*$3-2.*Tt*$4)/2)}' $snap \
	| ./d1 \
    	| awk 'NR==1 {print $2}')

    echo "$Tt $y" >> fit_two_lambda.dat

    #\
#	| awk -v "Tt=$Tt" '{printf("%.16G %.16G\n", Tt, $2); exit 0}' >> fit_two_lambda.dat

done
