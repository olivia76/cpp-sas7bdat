#!/usr/bin/env python
import pytest
import pycppsas7bdat

def test_version():
    assert pycppsas7bdat.getVersion() == "1.0.1"
