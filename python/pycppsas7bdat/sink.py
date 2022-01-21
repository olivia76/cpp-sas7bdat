import pandas as pd

class Sink(object):
    def __init__(self):
        self.rows = []
    
    def set_properties(self, properties):
        self.columns = [col.name for col in properties.metadata.columns]

    def push_row(self, irow, row):
        self.rows.append(row)

    @property
    def df(self):
        return pd.DataFrame.from_records(self.rows, columns = self.columns)

class SinkChunk(object):
    def __init__(self, chunk_size=10000):
        self.rows = []
        self.chunk_size = chunk_size
    
    def set_properties(self, properties):
        self.columns = [col.name for col in properties.metadata.columns]

    def push_rows(self, istartrow, iendrow, rows):
        rows = pd.DataFrame(rows, columns = self.columns)
        self.rows.append(rows)

    @property
    def df(self):
        return pd.concat(self.rows)
