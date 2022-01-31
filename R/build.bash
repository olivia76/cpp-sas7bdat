#!/bin/bash

echo "Compiling R package ..."
R_HOME=$(R RHOME | tail -1)
PKG=$($R_HOME/bin/Rscript -e 'cat("\nBEGIN\n"); sink(file="devtools-out.txt"); pkg=devtools::build(pkg="CPPSAS7BDAT", binary=TRUE, quiet=FALSE); sink(); cat("\nEND\n"); cat(pkg);' | tail -1)  #2>devtools-err.txt | tail -1)
R=$?
if [ $R -eq 0 ]; then
    echo "Installing [$PKG]..."
    Rscript -e 'install.packages("'$PKG'")'
else
    echo "Failed compilation..."
    cat devtools-out.txt
    cat devtools-err.txt
fi
