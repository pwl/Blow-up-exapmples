#!/bin/bash
# Interpolate data from stdin at point specified as the first argument ($1)

spline -t $1 $1 -n1 - |head -n1
