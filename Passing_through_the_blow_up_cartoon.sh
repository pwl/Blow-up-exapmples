#!/bin/bash

mplotx=
snapshot_dir="log/snapshot_multiple_blow-up_S3_S3/"
logfile="log/info_1/log000_multiple_blow-up_S3_S3.dat"
snapshot_dir="log/snapshot"
logfile="log/info_1/log000.dat"
snapshot_name="*.dat"
mrows=4
mcols=3
mtot=$((mcols*mrows))
mspace=$((mtot+4))
# snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
#     sort -n -t'_' -k2 | awk 'NR % 365 == 0 && NR >= 0' | head -n$mspace|awk 'NR <= 8 || NR >= 13')
# snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
#     sort -n -t'_' -k2 | ./select_by_time.sh .04 | head -n$mtot|awk 'NR%1==0 {print}')
# snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
#     sort -n -t'_' -k2 | ./select_by_time.sh .06 | head -n$mspace|awk 'NR <= 8 || NR >= 13')
# snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
#     sort -n -t'_' -k2 | ./select_by_time.sh .02 \
# 	| awk 'NR == 1 || NR == 4 || NR == 5 || NR == 7 || NR == 12 || NR == 13 || NR == 21 || NR == 31 || NR == 43 || NR == 44 || NR == 55 || NR == 85') #| head -n$mspace|awk 'NR <= 8 || NR >= 13')
# echo "$snapshot_files" > choice.dat
snapshot_files=$(cat choice.dat)
startx=0.1
starty=0.1
stopx=0.9
stopy=0.9
size_mult=1.
T=0.328075426868397

echo "" > plotter.gp

i=0

for snap in $snapshot_files; do

    t=$(awk '/t = /{printf("%.8f",$4)}' $snap)
    g=$(awk '/g = /{print $4}' $snap)
    gprint=$(awk '/g = /{printf("%1.2E\n",$4)}' $snap)
    s=$(awk '/s = /{printf("%.0f",$4)}' $snap)
    du=$(awk '/du = /{printf("%.0f",$4)}' $snap)
    T_t=$(echo "scale=20; $T-$t"|bc|\
awk '{e=int(log($1<0?-$1:$1)/log(10.))-1.; printf("%.1f\\cdot 10^{%i}",10.**(-e)*$1,e)}')

    echo "t$i=$t" >> plotter.gp
    echo "set origin mod($i,$mrows)*width,($mrows+1)*height-floor($i/$mrows)*height" >> plotter.gp
    # echo "set origin (mod($i,$mrows)+.25)*width,($mrows+1)*height-floor($i/$mrows)*height" >> plotter.gp

    echo 'set label 1 "$'$t'$" graph lposx, graph lposy' >> plotter.gp
    echo -ne 'plot parametric sin(t):cos(t) w l lw 1 lt 2, "'$snap'" u ((1.+.3*sin($1))*sin($2)):((1.+.3*sin($1))*cos($2)) w l lw 2 lt 1\n' >> plotter.gp

    i=$((i+1))
done

pyxplot Passing_through_the_blow_up_cartoon.gp
eps2eps graphics/Passing_through_the_blow_up_cartoon{,_opt}.eps

