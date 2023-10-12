#!/usr/bin/env python
import pytest
from pycppsas7bdat import Reader, Format, Compression
from pycppsas7bdat.sink import SinkByRow, SinkByChunk, SinkWholeData
from pycppsas7bdat.read_sas import read_sas
import os
import json
import datetime
import pandas as pd

__CUR_DIR = os.path.dirname(os.path.abspath(__file__))

def datafilename(f):
    return os.path.join(__CUR_DIR, f)

#@pytest.fixture()
def get_files():
    with open(datafilename("files.json")) as isf:
        files = json.load(isf)
        return files
        #return { k:v for k,v in files.items() if k in ('data_AHS2013/homimp.sas7bdat',) }

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

def check_properties(properties, ref_values):
    def get_u64(x):
        if x == True: return Format.bit64
        else: return Format.bit32

    def get_compression(x):
        if x == "SASYZCR2": return Compression.RDC
        if x == "SASYZCRL": return Compression.RLE
        return Compression.none

    def get_string(x):
        if x is None: return ""
        return x

    def get_dt(x):
        return x.strftime("%Y-%m-%d %H:%M:%S")

    def get_dt2(x):
        return x[:19]

    assert(properties.format == get_u64(ref_values["Header"]["u64"]))
    assert(str(properties.endianness) == ref_values["Header"]["endianess"])
    assert(str(properties.platform) == ref_values["Header"]["platform"])
    assert(get_dt(properties.date_created) == get_dt2(ref_values["Header"]["date_created"]))
    assert(get_dt(properties.date_modified) == get_dt2(ref_values["Header"]["date_modified"]))
    assert(properties.dataset_name == ref_values["Header"]["name"])
    #assert(properties.encoding == ref_values["Header"]["encoding"])
    assert(properties.file_type == ref_values["Header"]["file_type"])
    assert(properties.sas_release == ref_values["Header"]["sas_release"])
    assert(properties.sas_server_type == ref_values["Header"]["server_type"])
    assert(properties.os_type == ref_values["Header"]["os_type"])
    assert(properties.os_name == ref_values["Header"]["os_name"])
    assert(properties.header_length == ref_values["Header"]["header_length"])
    assert(properties.page_length == ref_values["Header"]["page_length"])
    assert(properties.page_count == ref_values["Header"]["page_count"])
    assert(properties.compression == get_compression(ref_values["Header"]["compression"]))
    assert(properties.creator == get_string(ref_values["Header"]["creator"]))
    assert(properties.creator_proc == get_string(ref_values["Header"]["creator_proc"]))
    assert(properties.row_length == ref_values["Header"]["row_length"])
    assert(properties.row_count == ref_values["Header"]["row_count"])
    assert(properties.column_count == ref_values["Header"]["column_count"])
    assert(properties.col_count_p1 == ref_values["Header"]["col_count_p1"])
    assert(properties.col_count_p2 == ref_values["Header"]["col_count_p2"])
    assert(properties.mix_page_row_count == ref_values["Header"]["mix_page_row_count"])
    assert(properties.lcs == ref_values["Header"]["lcs"])
    assert(properties.lcp == ref_values["Header"]["lcp"])


def check_sink(sink, ref_values):
    check_properties(sink.properties, ref_values)
    df = sink.df
    for irow, ref_row in ref_values["Data"].items():
        irow = int(irow)
        check_row(df.iloc[irow].tolist(), ref_row)

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
        check_properties(test.properties(), ref_values)
        test.read_all()
        check_sink(sink, ref_values)

class Test_read_sas(object):
    def test_read_sas(self, files):
        f, ref_values = files
        if f.find('big5') != -1: return
        if f.find('zero_variables') != -1: return
        f = datafilename(f)
        sink = read_sas(f)
        check_sink(sink, ref_values)

class Test_skip(object):

    # Need to use a lambda to create a new sink for each call
    @pytest.mark.parametrize("sink_factory", [
        lambda: SinkByRow(),
        lambda: SinkByChunk(),
        lambda: SinkWholeData()
        ])
    def test_skip(self, files, sink_factory):
        f, ref_values = files
        if f.find('big5') != -1: return
        if f.find('zero_variables') != -1: return
        f = datafilename(f)
        print(f)
        sink = sink_factory()
        test = Reader(f, sink)
        for irow, ref_row in ref_values["Data"].items():
            irow = int(irow)
            assert test.skip(irow - test.current_row_index) == True
            assert test.read_row() == True

        df = sink.df
        #print(df)
        for i, ref_row in enumerate(ref_values["Data"].values()):
            check_row(df.iloc[i].tolist(), ref_row)
        #check_sink(sink, ref_values)

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
        assert [c.name for c in sink.properties.columns] == ["RAS", "RAH", "RAD", "JRAS", "JRAD", "CONTROL"]

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
        assert [c.name for c in sink.properties.columns] == ["c1"]

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
        assert [c.name for c in sink.properties.columns] == ["q1", "c2", "q2"]
