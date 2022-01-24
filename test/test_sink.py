#!/usr/bin/env python
import pytest
from pycppsas7bdat import Reader
from pycppsas7bdat.sink import Sink, SinkChunk
import os
import json
import datetime
import pandas as pd

__CUR_DIR = os.path.dirname(os.path.abspath(__file__))

def datafilename(f):
    return os.path.join(__CUR_DIR, f)

@pytest.fixture()
def get_files():
    with open(datafilename("files.json")) as isf:
        return json.load(isf)

@pytest.fixture(params=get_files().items(), ids=get_files().keys())
def files(request):
    return request.param
    
def check_row(row, ref_row):
    for a, b in zip(row, ref_row):
        if b is None:
            assert pd.isnull(a)
        elif isinstance(a, datetime.datetime):
            assert(str(a) == b)
        elif isinstance(a, datetime.date):
            assert(a.isoformat() == b)
        elif isinstance(a, datetime.time):
            assert(a.isoformat() == b)
        elif isinstance(b, str):
            b = b.encode('utf8')
            assert(a == b)
        else:
            assert(a == b)            
    
class TestSink(object):

    def test_files_sink(self, files):
        f, ref_values = files
        if f.find('big5') != -1: return
        if f.find('zero_variables') != -1: return
        f = datafilename(f)
        print(f)
        sink = Sink()
        test = Reader(f, sink)
        test.read_all()
        df = sink.df
        for irow, ref_row in ref_values["Data"].items():
            irow = int(irow)
            check_row(df.iloc[irow].tolist(), ref_row)

    def test_files_chunksink(self, files):
        f, ref_values = files
        if f.find('big5') != -1: return
        if f.find('zero_variables') != -1: return
        f = datafilename(f)
        print(f)
        sink = SinkChunk()
        test = Reader(f, sink)
        test.read_all()
        df = sink.df
        for irow, ref_row in ref_values["Data"].items():
            irow = int(irow)
            check_row(df.iloc[irow].tolist(), ref_row)

