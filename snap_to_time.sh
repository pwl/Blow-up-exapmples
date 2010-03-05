#!/bin/bash

echo "$1" | gawk -f snap_to_time.awk
