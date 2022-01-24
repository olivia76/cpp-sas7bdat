#!/usr/bin/env python

from sas7bdat import SAS7BDAT
import sys

def main(inputfilename):
    sas = SAS7BDAT(inputfilename)
    for row in sas:
        pass

if __name__ == '__main__':
    main(sys.argv[1])
