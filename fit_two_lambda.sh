#!/bin/bash

snapshot_dir="log/snapshot"
logfile="log/info_1/log000.dat"
snapshot_name="*.dat"
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t'_' -k2  | awk 'NR % 100 == 0 && NR >= 0' ) #|head -n20 )
T=$(awk 'BEGIN {max=0} {if($2 > max) max=$2} END {printf("%.50f",max)}' $logfile)

# echo $T; exit 0;


rm -f fit_two_lambda.dat
rm -f time.dat

# for snap in $snapshot_files; do
#     t=$(awk '/t = /{printf( "%.50f", $4)}' $snap)
#     # Tt=$(awk -v "T=$T" '/t = /{printf("%.100f",T-$4);exit 0}' $snap)
#     Tt=$(echo "scale=50; $T-$t"|bc)
#     # Tt=$(echo "scale=20;$T-$t"|bc)

#     # echo "$snap"
#     # echo -ne "$(echo "scale=20; l(sqrt(if( $Tt > 0 ) $Tt else -$Tt))"|bc -l)\r"
#     # echo "$(echo "scale=20; a=$Tt; if( a < 0 ) a*=-1.; l(a)")"|bc -l
#     echo "$Tt"
#     echo "$t" >> time.dat

#     # calculate xdu/dx+2(T-t)du/dt
#     # first derivative
#     # take value at x=0
#     y=$(awk -v "Tt=$Tt" '/^[0-9].*/ && NR <=15 {printf("%.16G %.16G\n", $1/sqrt(Tt>0?Tt:-Tt), ($1*$3-2.*Tt*$4)/2)}' $snap \
#     	| ./d1 \
#     	| awk 'NR==1 {print $2}')

#     echo "$Tt $y" >> fit_two_lambda.dat

#     #\
# #	| awk -v "Tt=$Tt" '{printf("%.16G %.16G\n", Tt, $2); exit 0}' >> fit_two_lambda.dat

# done

rm -f plotter.gp
echo "T=$T" >> plotter.gp
echo 'file="'$logfile'"' >> plotter.gp

gnuplot -persist fit_two_lambda.gp
