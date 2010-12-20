#!/bin/bash

mplotx=
snapshot_dir="log/snapshot_single_blow-up_R3_to_R3/"
logfile="log/info_1/log000_single_blow-up_R3_to_R3.dat"
# snapshot_dir="log/snapshot/"
# logfile="log/info_1/log000.dat"
snapshot_name="*.dat"
mrows=3
mcols=3
mtot=$((mcols*mrows))
mspace=$((mtot))
snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
    sort -n -t '_' -k2 | awk 'NR > 700' | awk 'NR % 100 == 1' | head -n $mtot  )
# snapshot_files=$(find $snapshot_dir -name "$snapshot_name" |
#     sort -n -t'_' -k2 | awk 'NR % 1646 == 1' | head -n$mtot|awk 'NR%1==0 {print}')
blowup_file1="harvester_data_shrinker/shrinker_k3.00000_l1.0.dat"
# blowup_file2="harvester_data_shrinker/eigen_k3.00000_l1.0_i1.dat"
blowup_file2="harvester_data_expander/expander_k3.00000_l1.0.dat"
startx=0.1
starty=0.1
stopx=0.9
stopy=0.9
size_mult=1.
T=$(awk 'BEGIN {max=0} {if($2 > max) max=$2} END {printf("%.20f",max)}' $logfile)
T="0.01574053748"
# T=0.328077505829169
# T=0.328077505829110
# T=0.328075426868397

rm -f plotter.gp

i=0

for snap in $snapshot_files; do

    t=$(awk '/t = /{printf("%.20f",$4)}' $snap)
    g=$(awk '/g = /{print $4}' $snap)
    gprint=$(awk '/g = /{printf("%1.2E\n",$4)}' $snap)
    s=$(awk '/s = /{printf("%.0f",$4)}' $snap)
    du=$(awk '/du = /{printf("%.0f",$4)}' $snap)
    T_t=$(echo "scale=20; $T-$t"|bc|\
awk '{e=int(log($1<0?-$1:$1)/log(10.))-1.; printf("%.1f\\cdot 10^{%i}",10.**(-e)*$1,e)}')

    echo "set origin mod($i,$mrows)*width,-floor($i/$mrows)*height" >> plotter.gp

    tics=$(echo "($i%$mrows==0) && (($i/$mcols)+1==$mrows)"|bc)

#echo "set title \"T-t=$T_t\" offset screen .05*$sizex, screen -1.0*$sizey font \"Times-Roman,10\"" >> plotter.gp
    # echo -ne "plot [:pi] [ 0:pi ] \"$snap\" u (\$1/sqrt(abs($T-$t))):1 w l lw 2," >> plotter.gp
    echo 'set label 1 "\footnotesize{$T-t='$T_t'$}" graph 1e-1, graph 2.1' >> plotter.gp
    echo -ne "plot \"$snap\" u (\$1/sqrt(abs($T-$t))):(\$2/\$1) select (\$1>0) w l lw 2," >> plotter.gp
    echo -ne "\"$blowup_file1\" index 0 select (\$1 < 10) w l lt 2 lw 1," >> plotter.gp
    echo -ne "b1 select (x > 10) w l lt 2 lw 1\n" >> plotter.gp
# , pi w l lt 3 lw 1, x>10?b1:1/0 w l lt 2 lw 1 \n

    i=$((i+1))
done

echo "set output \"graphics/single_blowup_R3_to_S3_multiplot.eps\"" >> plotter.gp

# chmod a+x plotter.gp
# rm -f graphics/.eps
pyxplot Convergence_to_f1.gp
# okular graphics/Convergence_to_f1.eps
