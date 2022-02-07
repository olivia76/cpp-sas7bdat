import pandas as pd

class SinkBase(object):
    def __init__(self):
        self.properties = None
        self.columns = None
        
    def set_properties(self, properties):
        self.properties = properties
        self.columns = [col.name for col in properties.metadata.columns]

class Sink(SinkBase):
    def __init__(self):
        self.rows = []
    
    def push_row(self, irow, row):
        self.rows.append(row)

    @property
    def df(self):
        return pd.DataFrame.from_records(self.rows, columns = self.columns)

class SinkChunk(SinkBase):
    def __init__(self, chunk_size=10000):
        self.rows = []
        self.chunk_size = chunk_size

    def push_rows(self, istartrow, iendrow, rows):
        rows = pd.DataFrame(rows, columns = self.columns, copy=False)
        self.rows.append(rows)

    @property
    def df(self):
        return pd.concat(self.rows)

class SinkData(SinkBase):
    def __init__(self, chunk_size=10000):
        self.df = None

    def set_data(self, columns):
        self.df = pd.DataFrame(columns, columns=self.columns, copy=False)
        
