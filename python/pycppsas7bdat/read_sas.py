from pycppsas7bdat import Reader
from pycppsas7bdat.sink import SinkWholeData

def read_sas(inputfilename, sink=None):
    if sink is None: sink = SinkWholeData() 
    reader = Reader(inputfilename, sink)
    reader.read_all()
    return sink
