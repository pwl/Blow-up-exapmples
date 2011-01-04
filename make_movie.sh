#!/bin/sh

ls -1 $1/*.png|sort -t'_' -k2n > filelist.txt
mencoder -ovc lavc -lavcopts vbitrate=600 -mf type=png:fps=30 -o $1.avi mf://@filelist.txt
