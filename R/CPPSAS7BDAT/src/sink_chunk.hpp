/**
 *  \file sink_chunk.hpp
 *
 *  \brief  Reader export
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_R_SINK_CHUNK_HPP_
#define _CPP_SAS7BDAT_R_SINK_CHUNK_HPP_

#include "sink_by_columns.hpp"

namespace Rcppsas7bdat {

struct SinkChunk : public SinkByColumns {
  size_t istartrow{0}, iendrow{0};

  explicit SinkChunk(Environment _sink, const size_t _size)
      : SinkByColumns(_sink, _size), f_push_rows(sink["push_rows"]) {}

  void push_row([[maybe_unused]] const size_t _irow,
                [[maybe_unused]] cppsas7bdat::Column::PBUF _p) {
    SinkByColumns::push_row(_irow, _p);

    iendrow = _irow;
    if (idata == size) {
      flush();
      // Check if the were starting the last chunk => resize to a smaller chunk
      if (istartrow + size > ilastrow && istartrow < ilastrow) {
        prepare_values(ilastrow - istartrow);
      }
    }
  }

  void end_of_data() { flush(); }

  void flush() {
    if (istartrow > iendrow || idata == 0)
      return;
    SinkByColumns::set_values();
    f_push_rows(row_index, rows);
    idata = 0;
    istartrow = iendrow + 1;
  }

protected:
  Function f_push_rows;
};

} // namespace Rcppsas7bdat

#endif
