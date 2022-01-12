import pycppsas7bdat

print(pycppsas7bdat.getVersion())

class MySink(object):
    rows = []

    def set_properties(self, properties):
        pass

    def push_row(self, irow, row):
        self.rows.append(row)

s = MySink()        
r = pycppsas7bdat.Reader("../../test/data_AHS2013/topical.sas7bdat", s)
r.read_all()

