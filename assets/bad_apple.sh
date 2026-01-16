#!/bin/bash

for i in $(seq 0 6562):
do
	ascii-image-converter bad_apple/bad_apple_$(printf %03d $i).png -C --color-bg > ascii_apple/ascii_$(printf %05d $i).txt
done
