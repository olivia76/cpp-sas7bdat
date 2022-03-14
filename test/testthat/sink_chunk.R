#
# @author: Olivia Quinet
#
# @brief: Test the sinks for R
#

library(R6)

TestSinkChunk <- R6Class("TestSinkChunk",
     public=list(
        chunk_size=10000,
        columns = NULL,
	columns_comparison = NULL,
	ref = NULL,
	ref_irows = NULL,
	ref_idata = 0,
	ref_irow = NULL,
        initialize = function(ref) {
	self$ref = ref;
        },
        set_properties = function(properties) {
	  #print(properties$columns);
	  #print(self$ref$Columns)
	  
	  check_properties(properties, self$ref);
	  self$columns_comparison = get_columns_comparison(properties$columns);
	  self$columns = names(self$columns_comparison)
	  
	  self$ref_idata = 1;
	  self$ref_irows = as.integer(names(self$ref$Data))
	  self$ref_irow = self$ref_irows[self$ref_idata];
        },
        push_rows = function(row_index, rows) {
          #print(paste("TestSinkChunk$push_rows: rows=", row_index, rows));
	  i=0;
	  for(irow in row_index) {
	    i=i+1;
	    if(irow == self$ref_irow) {
    	      #print(irow);
	      ref_row = as.list(self$ref$Data[[self$ref_idata]]);
	      names(ref_row) = self$columns;
	      #print(ref_row);

	      for(icol in seq_along(self$columns_comparison)) {
	        colname = self$columns[[icol]]
	      	col = unname(unlist(rows[colname]))
		#print(icol)
		#print(colname)
	        #print(col)
    	        self$columns_comparison[[icol]](col[[i]], ref_row[[icol]]);
	      }

	      self$ref_idata = self$ref_idata+1;
	      self$ref_irow = self$ref_irows[self$ref_idata];
	    }
	  }
        }
     )
);
