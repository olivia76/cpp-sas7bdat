#
# @author: Olivia Quinet
#
# @brief: Sink classes

library(R6)

Sink <- R6Class("Sink",
     public=list(
	df = NULL,
	properties = NULL,
	columns = NULL,
        initialize = function() {
        },
        set_properties = function(properties) {
	  self$properties = properties;
	  self$columns = names(properties$metadata$columns);
	  self$df = data.frame(matrix(NA, nrow=properties$metadata$row_count, ncol=properties$metadata$column_count))
	  names(self$df) = self$columns 
        },
        push_row = function(irow, row) {
	  for(icol in seq_along(self$columns)) {
	    if(!is.null(row[[icol]])) self$df[irow+1, icol] = row[[icol]];
	  }
        }
     )
);

SinkChunk <- R6Class("SinkChunk",
     public=list(
	df = NULL,
	properties = NULL,
	columns = NULL,
        chunk_size = NULL,
        initialize = function(chunk_size=10000) {
            self$chunk_size = chunk_size;
        },
        set_properties = function(properties) {
	  self$properties = properties;
	  self$columns = names(properties$metadata$columns);
        },
        push_rows = function(istartrow, iendrow, rows) {
	  rows <- as.data.frame(rows);
	  if(is.null(self$df)) { self$df=rows; }
	  else { self$df = rbind(self$df, rows); }
        }
     )
);

SinkData <- R6Class("SinkData",
     public=list(
	df = NULL,
	properties = NULL,
	columns = NULL,
        initialize = function() {
        },
        set_properties = function(properties) {
	  self$properties = properties;
	  self$columns = names(properties$metadata$columns);
        },
        set_data = function(data) {
	  self$df <- data;
        }
     )
);

