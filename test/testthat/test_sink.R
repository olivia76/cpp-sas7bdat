#
# @author: Olivia Quinet
#
# @brief: Test the sinks for R
#

context("Sink")
library(R6)

source("compare.R")

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
	  #print(properties$metadata$columns);
	  #print(self$ref$Columns)

	  check_properties(properties, self$ref);
	  self$columns_comparison = get_columns_comparison(properties$metadata$columns);
	  self$columns = names(self$columns_comparison)
	  
	  self$ref_idata = 1;
	  self$ref_irows = as.integer(names(self$ref$Data))
	  self$ref_irow = self$ref_irows[self$ref_idata];
        },
        push_row = function(irow, row) {
	  if(irow == self$ref_irow) {
    	    #print(irow);
	    ref_row = as.list(self$ref$Data[[self$ref_idata]]);
	    names(ref_row) = self$columns;
	    #print(ref_row);
	    #print(row);

	    for(icol in seq_along(self$columns_comparison)) {
	      #print(icol)
    	      self$columns_comparison[[icol]](row[[icol]], ref_row[[icol]]);
	    }

	    self$ref_idata = self$ref_idata+1;
	    self$ref_irow = self$ref_irows[self$ref_idata];
	  }
        }
     )
);

test_that("I can read a SAS7BDAT file with the default Sink", {
    for(file in names(files)) {
    #file="data_pandas/datetime.sas7bdat"; {
        ref = files[[file]]
        file=paste0("../", file);
	print(paste("Default sink:", file));
      	sink = TestSink$new(ref);
        reader <- CPPSAS7BDAT::sas_reader(file, sink);
	# Skip big5 and test16: json unicode cannot be decoded correctly 
	if(grepl("big5", file, fixed=TRUE) == FALSE &&
	   grepl("test16", file, fixed=TRUE) == FALSE) {
            reader$read_all();
	}
       rm(reader);
       rm(sink);
    }
})

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
	  #print(properties$metadata$columns);
	  #print(self$ref$Columns)
	  
	  check_properties(properties, self$ref);
	  self$columns_comparison = get_columns_comparison(properties$metadata$columns);
	  self$columns = names(self$columns_comparison)
	  
	  self$ref_idata = 1;
	  self$ref_irows = as.integer(names(self$ref$Data))
	  self$ref_irow = self$ref_irows[self$ref_idata];
        },
        push_rows = function(istartrow, iendrow, rows) {
	  #print(rows)
	  for(irow in seq(istartrow, iendrow)) {
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
    	        self$columns_comparison[[icol]](col[[irow-istartrow+1]], ref_row[[icol]]);
	      }

	      self$ref_idata = self$ref_idata+1;
	      self$ref_irow = self$ref_irows[self$ref_idata];
	    }
	  }
        }
     )
);

test_that("I can read a SAS7BDAT file with the Chunk Sink", {
    for(file in names(files)) {
    #file="data_pandas/datetime.sas7bdat"; {
        ref = files[[file]]
        file=paste0("../", file);
	print(paste("Chunk sink:", file));
      	sink = TestSinkChunk$new(ref);
        reader = CPPSAS7BDAT::sas_reader(file, sink);
	# Skip big5 and test16: json unicode cannot be decoded correctly 
	if(grepl("big5", file, fixed=TRUE) == FALSE &&
	   grepl("test16", file, fixed=TRUE) == FALSE) {
            reader$read_all();
	}
        rm(reader);
        rm(sink);
    }
})

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
	  #print(properties$metadata$columns);
	  #print(self$ref$Columns)

	  self$row_count = properties$metadata$row_count;
	  
	  check_properties(properties, self$ref);
	  self$columns_comparison = get_columns_comparison(properties$metadata$columns);
	  self$columns = names(self$columns_comparison)
	  
	  self$ref_idata = 1;
	  self$ref_irows = as.integer(names(self$ref$Data))
	  self$ref_irow = self$ref_irows[self$ref_idata];
        },
        set_data = function(rows) {
	  #print(rows)
	  for(irow in seq(0, self$row_count-1)) {
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
    	        self$columns_comparison[[icol]](col[[irow+1]], ref_row[[icol]]);
	      }

	      self$ref_idata = self$ref_idata+1;
	      self$ref_irow = self$ref_irows[self$ref_idata];
	    }
	  }
        }
     )
);

test_that("I can read a SAS7BDAT file with the Data Sink", {
    for(file in names(files)) {
    #file="data_pandas/datetime.sas7bdat"; {
        ref = files[[file]]
        file=paste0("../", file);
	print(paste("Data sink", file));
      	sink = TestSinkData$new(ref);
        reader = CPPSAS7BDAT::sas_reader(file, sink);
	# Skip big5 and test16: json unicode cannot be decoded correctly 
	if(grepl("big5", file, fixed=TRUE) == FALSE &&
	   grepl("test16", file, fixed=TRUE) == FALSE) {
            reader$read_all();
	}
        rm(reader);
        rm(sink);
    }
})