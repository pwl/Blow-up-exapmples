#!/usr/bin/igawk -f

@include helpers.awk

/^[0-9]/ && $1 != 0 {
    printf("%.20G %.20G\n",
	   $1/sqrt(abs(T-t)), ($1*$3-2*abs(T-t)*$4-$2)/$1)
  }
