#!/bin/bash

if [ "$1" == "RCPP_INC" ]; then
    DIRS=$($R_HOME/bin/Rscript -e 'cat(R.home("include"))' | tail -1)" "$($R_HOME/bin/Rscript -e 'Rcpp:::CxxFlags()' | cut -d\" -f2)
    echo $DIRS
fi
