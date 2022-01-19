#!/usr/bin/env python
import pytest
import pycppsas7bdat

def test_version():
    assert pycppsas7bdat.getVersion() != ""
