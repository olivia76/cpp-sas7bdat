#!/bin/bash

CURDIR=$(pwd)

while read -r f
do
    echo $f;
    if [[ ${f:0:1} == '#' ]]; then
	dir=${f#* }
	if [[ ! -z "$dir" ]]; then
	    echo "Creating directory [$dir] ..."
	    mkdir -p $CURDIR/$dir
	    cd $CURDIR/$dir
	fi
    else
	wget $f 2> /dev/null
	if [[ $? -ne 0 ]]; then
	    echo "Didn't manage to retrieve $f..."
	fi
    fi
done < files.txt
