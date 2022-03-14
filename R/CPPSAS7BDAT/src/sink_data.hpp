/**
 *  \file sink_chunk.hpp
 *
 *  \brief  Reader export
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_R_SINK_DATA_HPP_
#define _CPP_SAS7BDAT_R_SINK_DATA_HPP_

#include "sink_by_columns.hpp"

namespace Rcppsas7bdat {

struct SinkData : public SinkByColumns {
  SinkData(Environment _sink)
      : SinkByColumns(_sink, 0), f_set_data(sink["set_data"]) {}

  void end_of_data() { flush(); }

  void flush() {
    if (idata == 0)
      return;
    SinkByColumns::set_values();
    f_set_data(row_index, rows);
  }

protected:
  Function f_set_data;
};

} // namespace Rcppsas7bdat

#endif
