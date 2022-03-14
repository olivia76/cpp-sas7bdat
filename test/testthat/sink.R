#
# @author: Olivia Quinet
#
# @brief: Test the sinks for R
#

library(R6)

TestSink <- R6Class("TestSink",
     public=list(
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
	  #print(paste("TestSink$set_properties", properties$columns));

	  check_properties(properties, self$ref);
	  self$columns_comparison = get_columns_comparison(properties$columns);
	  self$columns = names(self$columns_comparison)
	  
	  self$ref_idata = 1;
	  self$ref_irows = as.integer(names(self$ref$Data))
	  self$ref_irow = self$ref_irows[self$ref_idata];
        },
        push_row = function(irow, row) {
	  if(irow == self$ref_irow) {
	    #print(paste("TestSink$push_row: irow=", irow));
    	    #print(irow);
	    ref_row = as.list(self$ref$Data[[self$ref_idata]]);
	    names(ref_row) = self$columns;
	    #print(ref_row);
	    #print(row);

	    for(icol in seq_along(self$columns_comparison)) {
 	      #print(paste("TestSink$push_row: icol=", icol));
    	      self$columns_comparison[[icol]](row[[icol]], ref_row[[icol]]);
	    }

	    self$ref_idata = self$ref_idata+1;
	    self$ref_irow = self$ref_irows[self$ref_idata];
	  }
        }
     )
);

