#!/bin/sh

ls -1 movie/*.png|sort -t'_' -k2n > filelist.txt
mencoder -ovc lavc -lavcopts vbitrate=800 -mf type=png:fps=30 -o movie.avi mf://@filelist.txt
