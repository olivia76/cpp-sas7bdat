#!/bin/bash

echo "Compiling R package ..."
set -o pipefail
R_HOME=$(R RHOME | tail -1)
PKG=$($R_HOME/bin/Rscript -e 'cat("\nBEGIN\n"); sink(file="devtools-out.txt"); pkg=devtools::build(pkg="CPPSAS7BDAT", binary=TRUE, quiet=FALSE); sink(); cat("\nEND\n"); cat(pkg);' 2>devtools-err.txt | tail -1)
R=$?
if [ $R -eq 0 ]; then
    echo "Installing [$PKG]..."
    Rscript -e 'install.packages("'$PKG'")'
else
    echo "Failed compilation..."
    echo "out:"
    cat devtools-out.txt
    echo "err:"
    cat devtools-err.txt
fi
