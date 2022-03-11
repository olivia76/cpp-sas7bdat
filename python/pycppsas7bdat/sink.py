import pandas as pd

class SinkBase(object):
    def __init__(self):
        self.properties = None
        self.columns = None
        
    def set_properties(self, properties):
        self.properties = properties
        self.columns = [col.name for col in properties.columns]

    def _cpp_flush_sink(self):
        if hasattr(self, "_cpp"): self._cpp.flush_sink()        
        
class SinkByRow(SinkBase):
    def __init__(self):
        super().__init__()
        self.rows = []
    
    def push_row(self, irow, row):
        self.rows.append(row)

    @property
    def df(self):
        self._cpp_flush_sink()
        return pd.DataFrame.from_records(self.rows, columns = self.columns)

class SinkByChunk(SinkBase):
    def __init__(self, chunk_size=10000):
        super().__init__()
        self.rows = []
        self.chunk_size = chunk_size

    def push_rows(self, istartrow, iendrow, rows):
        rows = pd.DataFrame(rows, columns = self.columns, copy=False)
        self.rows.append(rows)

    @property
    def df(self):
        self._cpp_flush_sink()
        return pd.concat(self.rows)

class SinkWholeData(SinkBase):
    def __init__(self):
        super().__init__()
        self._df = None

    def set_data(self, columns):
        self._df = pd.DataFrame(columns, columns=self.columns, copy=False)
        
    @property
    def df(self):
        self._cpp_flush_sink()
        return self._df
    
