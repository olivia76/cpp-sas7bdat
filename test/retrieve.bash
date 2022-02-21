#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ -z "$WGET" ]; then
    WGET=wget
fi

while read -r f
do
    echo $f;
    if [[ ${f:0:1} == '#' ]]; then
	dir=${f#* }
	if [[ ! -z "$dir" ]]; then
	    echo "Creating directory [$dir] ..."
	    mkdir -p $DIR/$dir
	    cd $DIR/$dir
	fi
    else
	$WGET $f 2> /dev/null
	if [[ $? -ne 0 ]]; then
	    echo "Didn't manage to retrieve $f..."
	fi
    fi
done < $DIR/files.txt
