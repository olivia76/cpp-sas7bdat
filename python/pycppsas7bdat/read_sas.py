# -*- coding: utf-8 -*-
# @brief: Wrapper for the C++ classes 
# @author: Olivia Quinet
#

from pycppsas7bdat import Reader
from pycppsas7bdat.sink import SinkWholeData

def read_sas(inputfilename, sink=None, include=None, exclude=None):
    if sink is None: sink = SinkWholeData()
    reader = Reader(inputfilename, sink, include=include, exclude=exclude)
    reader.read_all()
    return sink
