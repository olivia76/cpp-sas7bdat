#
# @author: Olivia Quinet
#
# @brief: Test the sinks for R
#

library(R6)

TestSinkData <- R6Class("TestSinkData",
     public=list(
        columns = NULL,
	columns_comparison = NULL,
	ref = NULL,
	ref_irows = NULL,
	ref_idata = 0,
	ref_irow = NULL,
	row_count = 0,
        initialize = function(ref) {
  	  self$ref = ref;
        },
        set_properties = function(properties) {
	  #print(properties$columns);
	  #print(self$ref$Columns)

	  self$row_count = properties$row_count;
	  
	  check_properties(properties, self$ref);
	  self$columns_comparison = get_columns_comparison(properties$columns);
	  self$columns = names(self$columns_comparison)
	  
	  self$ref_idata = 1;
	  self$ref_irows = as.integer(names(self$ref$Data))
	  self$ref_irow = self$ref_irows[self$ref_idata];
        },
        set_data = function(row_index, rows) {
	  #print(paste("TestSinkData$set_data: row_index=", row_index));
	  #print(paste("TestSinkData$set_data: rows=", rows));
	  i = 0
	  for(irow in row_index) {
	    i = i+1
	    if(irow == self$ref_irow) {
    	      #print(paste("TestSinkData$set_data: irow=", irow));
	      ref_row = as.list(self$ref$Data[[self$ref_idata]]);
	      names(ref_row) = self$columns;
	      #rint(paste("TestSinkData$set_data: ref_row=", ref_row));

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
