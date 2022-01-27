import pandas as pd
import sys, os

def main(inputfilename):
    pd.read_sas(inputfilename)
            
if __name__ == "__main__":
    main(sys.argv[1])
