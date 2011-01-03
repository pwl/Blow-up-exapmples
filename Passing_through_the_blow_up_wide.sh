#!/bin/bash

snapshot_dir="log/snapshot_multiple_blow-up_S3_S3/"
logfile="log/info_1/log000_multiple_blow-up_S3_S3.dat"
snapshot_name="*.dat"
mrows=3
mcols=3
mtot=$((mcols*mrows))
mspace=$((mtot+4))
# snapshot_files=$( find $snapshot_dir -name "$snapshot_name" |
#     sort -n -t'_' -k2 | awk 'NR % 450 == 1 && NR >= 0' | head -n$mspace )
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t'_' -k2 | ./select_by_time.sh .02 \
	| awk 'NR == 1 || NR == 4 || NR == 5 || NR == 12 || NR == 13 || NR == 21 || NR == 43 || NR == 44 || NR == 85') #| head -n$mspace|awk 'NR <= 8 || NR >= 13')

echo "$snapshot_files" > choice.dat
# snapshot_files=$(cat choice.dat)
# echo "$snapshot_files"
exit 0

echo "" > plotter.gp
echo "" > plotter2.gp

i=1

for snap in $snapshot_files; do

    t=$(awk '/t = /{printf("%.2f",$4)}' $snap)

    echo "t$i=$t" >> plotter.gp
    echo "set origin (mod($i-1,$mrows)+.27)*width,($mrows+1)*height-floor(($i-1)/$mrows)*height" >> plotter.gp
    echo 'set label 1 "$'$t'$" graph lposx, graph lposy' >> plotter.gp
    echo 'plot "'$snap'" w l lw 2 lt 1' >> plotter.gp

    echo "t$i=$t" >> plotter2.gp
    echo "set origin (mod($i-1,$mrows)+.27)*width,($mrows+1)*height-floor(($i-1)/$mrows)*height" >> plotter2.gp
    echo 'set label 1 "$'$t'$" graph lposx, graph lposy' >> plotter2.gp
    echo 'plot parametric sin(t):cos(t) w l lw 1 lt 2, "'$snap'" u ((1.+.3*sin($1))*sin($2)):((1.+.3*sin($1))*cos($2)) w l lw 2 lt 1' >> plotter2.gp

    i=$((i+1))
done

pyxplot Passing_through_the_blow_up_wide.gp
eps2eps graphics/Passing_through_the_blow_up_wide{,_opt}.eps
eps2eps graphics/Passing_through_the_blow_up_cartoon{,_opt}.eps

