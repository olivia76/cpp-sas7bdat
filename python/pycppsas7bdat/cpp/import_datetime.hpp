namespace pycppsas7bdat {

  void bind_datetime();

  PyObject* to_python(boost::posix_time::ptime const& pt);
  PyObject* to_python(boost::posix_time::time_duration d);
  PyObject* to_python(boost::gregorian::date const& date);
}
