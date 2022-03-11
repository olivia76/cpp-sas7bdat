import pycppsas7bdat
from pycppsas7bdat.sink import SinkByRow, SinkByChunk, SinkWholeData
import sys, getopt

print(pycppsas7bdat.getVersion())

class MySink(object):
    def __init__(self):
        self.rows = []
    
    def set_properties(self, properties):
        self.columns = [col.name for col in properties.columns]

    def push_row(self, irow, row):
        self.rows.append(row)

    @property
    def df(self):
        return self.rows

class MySinkChunk(object):
    def __init__(self, chunk=10000):
        self.rows = []
        self.chunk_size = chunk
    
    def set_properties(self, properties):
        self.columns = [col.name for col in properties.columns]

    def push_rows(self, istartrow, iendrow, rows):
        self.rows.append(rows)
   
    @property
    def df(self):
        return self.rows
    
def main(argv):
    try:
        opts, args = getopt.getopt(argv,
                                   "f:s:",
                                   ["file=", "sink="])
    except getopt.GetoptError:
        sys.exit(2)

    for opt, arg in opts:
        if opt in ('-f', '--file'):
            inputfilename = arg
        elif opt in ('-s', '--sink'):
            sinktype = arg

    sink = {
        'sink': MySink(),
        'chunk': MySinkChunk(),
        'pd_sink': pycppsas7bdat.sink.SinkByRow(),
        'pd_chunk': pycppsas7bdat.sink.SinkByChunk(),
        'pd_data': pycppsas7bdat.sink.SinkWholeData(),
    }[sinktype]
    reader = pycppsas7bdat.Reader(inputfilename, sink)
    reader.read_all()
    sink.df
            
if __name__ == "__main__":
    main(sys.argv[1:])
