#!/usr/bin/env python
import pytest
from pycppsas7bdat import Reader
from pycppsas7bdat.sink import SinkByRow, SinkByChunk, SinkWholeData
from pycppsas7bdat.read_sas import read_sas
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
            assert(a.strftime("%Y-%m-%d %H:%M:%S.%f") == b)
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

    # Need to use a lambda to create a new sink for each call
    @pytest.mark.parametrize("sink_factory", [
        lambda: SinkByRow(),
        lambda: SinkByChunk(),
        lambda: SinkWholeData()
        ])
    def test_files_sink(self, files, sink_factory):
        f, ref_values = files
        if f.find('big5') != -1: return
        if f.find('zero_variables') != -1: return
        f = datafilename(f)
        print(f)
        sink = sink_factory()
        test = Reader(f, sink)
        test.read_all()
        df = sink.df
        for irow, ref_row in ref_values["Data"].items():
            irow = int(irow)
            check_row(df.iloc[irow].tolist(), ref_row)

class Test_read_sas(object):
    def test_read_sas(self, files):
        f, ref_values = files
        if f.find('big5') != -1: return
        if f.find('zero_variables') != -1: return
        f = datafilename(f)
        sink = read_sas(f)
        df = sink.df
        for irow, ref_row in ref_values["Data"].items():
            irow = int(irow)
            check_row(df.iloc[irow].tolist(), ref_row)
        
class Test_IncludeExclude(object):
    
    @pytest.mark.parametrize("sink_factory", [
        lambda: SinkByRow(),
        lambda: SinkByChunk(),
        lambda: SinkWholeData()
        ])
    def test_NoIncludeExclude(self, sink_factory):
        f = "data_AHS2013/homimp.sas7bdat"
        f = datafilename(f)
        print(f)
        sink = sink_factory()
        test = Reader(f, sink, include=None, exclude=None)
        test.read_all()
        assert [c.name for c in sink.properties.metadata.columns] == ["RAS", "RAH", "RAD", "JRAS", "JRAD", "CONTROL"]
    
    @pytest.mark.parametrize("sink_factory", [
        lambda: SinkByRow(),
        lambda: SinkByChunk(),
        lambda: SinkWholeData()
        ])
    def test_BadInclude(self, sink_factory):
        f = "data_AHS2013/homimp.sas7bdat"
        f = datafilename(f)
        print(f)
        sink = sink_factory()
        with pytest.raises(Exception) as e:
            test = Reader(f, sink, include="A")
        with pytest.raises(Exception) as e:
            test = Reader(f, sink, include=["A", None])
            
    @pytest.mark.parametrize("sink_factory", [
        lambda: SinkByRow(),
        lambda: SinkByChunk(),
        lambda: SinkWholeData()
        ])
    def test_BadExclude(self, sink_factory):
        f = "data_AHS2013/homimp.sas7bdat"
        f = datafilename(f)
        print(f)
        sink = sink_factory()
        with pytest.raises(Exception) as e:
            test = Reader(f, sink, exclude="A")
        with pytest.raises(Exception) as e:
            test = Reader(f, sink, exclude=["A", 1])
    
    @pytest.mark.parametrize("sink_factory", [
        lambda: SinkByRow(),
        lambda: SinkByChunk(),
        lambda: SinkWholeData()
        ])
    def test_Include(self, sink_factory):
        f = "data/file2.sas7bdat"
        f = datafilename(f)
        print(f)
        sink = sink_factory()
        test = Reader(f, sink, include=["c1",])
        test.read_all()
        assert [c.name for c in sink.properties.metadata.columns] == ["c1"]
    
    @pytest.mark.parametrize("sink_factory", [
        lambda: SinkByRow(),
        lambda: SinkByChunk(),
        lambda: SinkWholeData()
        ])
    def test_Exclude(self, sink_factory):
        f = "data/file2.sas7bdat"
        f = datafilename(f)
        print(f)
        sink = sink_factory()
        test = Reader(f, sink, exclude=["c1", ])
        test.read_all()
        assert [c.name for c in sink.properties.metadata.columns] == ["q1", "c2", "q2"]
