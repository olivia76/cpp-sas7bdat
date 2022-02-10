#
# @author: Olivia Quinet
#
# @brief: Test the sinks for R
#

context("Sink")
library("RJSONIO")
library(R6)

files = RJSONIO::fromJSON("../files.json")

ts <- function(x) { substr(x, 0, 19); }
str <- function(x) { if(is.null(x)) { ""; } else { x; }; };
compression <- function(x) { if(is.null(x)) { "none"; } else if(x == "SASYZCRL") { "RLE"; } else { "RDC"; } };
cf <- function(x) { if(x == "string") { x; } else { "notstring"; } };


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

	  expect_equal(properties$header$endianness, self$ref$Header$endianess);
	  expect_equal(properties$header$platform, self$ref$Header$platform);
	  expect_equal(ts(properties$header$date_created), ts(self$ref$Header$date_created));
	  expect_equal(ts(properties$header$date_modified), ts(self$ref$Header$date_modified));
	  expect_equal(properties$header$dataset_name, self$ref$Header$name);
	  #expect_equal(properties$header$encoding, self$ref$Header$encoding);
	  expect_equal(properties$header$file_type, self$ref$Header$file_type);
	  expect_equal(properties$header$sas_release, self$ref$Header$sas_release);
	  expect_equal(properties$header$sas_server_type, self$ref$Header$server_type);
	  expect_equal(properties$header$os_type, self$ref$Header$os_type);
	  expect_equal(properties$header$os_name, self$ref$Header$os_name);
	  expect_equal(properties$header$header_length, self$ref$Header$header_length);
	  expect_equal(properties$header$page_length, self$ref$Header$page_length);
	  expect_equal(properties$header$page_count, self$ref$Header$page_count);
	  expect_equal(properties$metadata$compression, compression(self$ref$Header$compression));
	  expect_equal(properties$metadata$creator, str(self$ref$Header$creator));
	  expect_equal(properties$metadata$creator_proc, str(self$ref$Header$creator_proc));
	  expect_equal(properties$metadata$row_length, self$ref$Header$row_length);
	  expect_equal(properties$metadata$row_count, self$ref$Header$row_count);
	  expect_equal(properties$metadata$column_count, self$ref$Header$column_count);
	  expect_equal(properties$metadata$col_count_p1, self$ref$Header$col_count_p1);
	  expect_equal(properties$metadata$col_count_p2, self$ref$Header$col_count_p2);
	  expect_equal(properties$metadata$mix_page_row_count, self$ref$Header$mix_page_row_count);
	  expect_equal(properties$metadata$lcs, self$ref$Header$lcs);
	  expect_equal(properties$metadata$lcp, self$ref$Header$lcp);

          self$columns = as.character(c(names(properties$metadata$columns)));
	  ref_columns = as.character(c(sapply(self$ref$Columns, FUN=function(x) { x$name; } )));
	  #print(self$columns);
	  #print(ref_columns);
	  expect_equal(self$columns, ref_columns);

	  ctype = unname(as.vector(sapply(properties$metadata$columns, FUN=function(x) { cf(x$type); })));
	  rtype = as.vector(sapply(self$ref$Columns, FUN=function(x) { cf(x$type); }));
	  #print(ctype);
	  #print(rtype);
	  expect_equal(ctype, rtype);
	  cformat = unname(as.vector(sapply(properties$metadata$columns, FUN=function(x) { x$format; })));
	  rformat = as.vector(sapply(self$ref$Columns, FUN=function(x) { x$format; }));
	  #print(cformat);
	  #print(rformat);
	  expect_equal(cformat, rformat);

	  self$columns_comparison = sapply(properties$metadata$columns, FUN=function(x) {
  	     function(x, y) {
	       #print(x); print(y);
	       if(is.null(x)) { expect_null(y); }
	       else if(is.na(x)) { expect_null(y); }
	       else { expect_equal(x, y); }
	     };
	  });
	  #print(self$columns_comparison);
	  
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
        ref = files[[file]]
        file=paste0("../", file);
	print(paste("Default sink:", file));
      	sink = TestSink$new(ref);
        reader = CPPSAS7BDAT::Reader(file, sink);
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
	
	  expect_equal(properties$header$endianness, self$ref$Header$endianess);
	  expect_equal(properties$header$platform, self$ref$Header$platform);
	  expect_equal(ts(properties$header$date_created), ts(self$ref$Header$date_created));
	  expect_equal(ts(properties$header$date_modified), ts(self$ref$Header$date_modified));
	  expect_equal(properties$header$dataset_name, self$ref$Header$name);
	  #expect_equal(properties$header$encoding, self$ref$Header$encoding);
	  expect_equal(properties$header$file_type, self$ref$Header$file_type);
	  expect_equal(properties$header$sas_release, self$ref$Header$sas_release);
	  expect_equal(properties$header$sas_server_type, self$ref$Header$server_type);
	  expect_equal(properties$header$os_type, self$ref$Header$os_type);
	  expect_equal(properties$header$os_name, self$ref$Header$os_name);
	  expect_equal(properties$header$header_length, self$ref$Header$header_length);
	  expect_equal(properties$header$page_length, self$ref$Header$page_length);
	  expect_equal(properties$header$page_count, self$ref$Header$page_count);
	  expect_equal(properties$metadata$compression, compression(self$ref$Header$compression));
	  expect_equal(properties$metadata$creator, str(self$ref$Header$creator));
	  expect_equal(properties$metadata$creator_proc, str(self$ref$Header$creator_proc));
	  expect_equal(properties$metadata$row_length, self$ref$Header$row_length);
	  expect_equal(properties$metadata$row_count, self$ref$Header$row_count);
	  expect_equal(properties$metadata$column_count, self$ref$Header$column_count);
	  expect_equal(properties$metadata$col_count_p1, self$ref$Header$col_count_p1);
	  expect_equal(properties$metadata$col_count_p2, self$ref$Header$col_count_p2);
	  expect_equal(properties$metadata$mix_page_row_count, self$ref$Header$mix_page_row_count);
	  expect_equal(properties$metadata$lcs, self$ref$Header$lcs);
	  expect_equal(properties$metadata$lcp, self$ref$Header$lcp);

          self$columns = as.character(c(names(properties$metadata$columns)));
	  ref_columns = as.character(c(sapply(self$ref$Columns, FUN=function(x) { x$name; } )));
	  #print(self$columns);
	  #print(ref_columns);
	  expect_equal(self$columns, ref_columns);

	  ctype = unname(as.vector(sapply(properties$metadata$columns, FUN=function(x) { cf(x$type); })));
	  rtype = as.vector(sapply(self$ref$Columns, FUN=function(x) { cf(x$type); }));
	  #print(ctype);
	  #print(rtype);
	  expect_equal(ctype, rtype);
	  cformat = unname(as.vector(sapply(properties$metadata$columns, FUN=function(x) { x$format; })));
	  rformat = as.vector(sapply(self$ref$Columns, FUN=function(x) { x$format; }));
	  #print(cformat);
	  #print(rformat);
	  expect_equal(cformat, rformat);

	  self$columns_comparison = sapply(properties$metadata$columns, FUN=function(x) {
  	     function(x, y) {
	       #print(x); print(y);
	       if(is.null(x)) { expect_null(y); }
	       else if(is.na(x)) { expect_null(y); }
	       else { expect_equal(x, y); }
	     };
	  });
	  #print(self$columns_comparison);
	  
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
        ref = files[[file]]
        file=paste0("../", file);
	print(paste("Chunk sink:", file));
      	sink = TestSinkChunk$new(ref);
        reader = CPPSAS7BDAT::Reader(file, sink);
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
	
	  expect_equal(properties$header$endianness, self$ref$Header$endianess);
	  expect_equal(properties$header$platform, self$ref$Header$platform);
	  expect_equal(ts(properties$header$date_created), ts(self$ref$Header$date_created));
	  expect_equal(ts(properties$header$date_modified), ts(self$ref$Header$date_modified));
	  expect_equal(properties$header$dataset_name, self$ref$Header$name);
	  #expect_equal(properties$header$encoding, self$ref$Header$encoding);
	  expect_equal(properties$header$file_type, self$ref$Header$file_type);
	  expect_equal(properties$header$sas_release, self$ref$Header$sas_release);
	  expect_equal(properties$header$sas_server_type, self$ref$Header$server_type);
	  expect_equal(properties$header$os_type, self$ref$Header$os_type);
	  expect_equal(properties$header$os_name, self$ref$Header$os_name);
	  expect_equal(properties$header$header_length, self$ref$Header$header_length);
	  expect_equal(properties$header$page_length, self$ref$Header$page_length);
	  expect_equal(properties$header$page_count, self$ref$Header$page_count);
	  expect_equal(properties$metadata$compression, compression(self$ref$Header$compression));
	  expect_equal(properties$metadata$creator, str(self$ref$Header$creator));
	  expect_equal(properties$metadata$creator_proc, str(self$ref$Header$creator_proc));
	  expect_equal(properties$metadata$row_length, self$ref$Header$row_length);
	  expect_equal(properties$metadata$row_count, self$ref$Header$row_count);
	  expect_equal(properties$metadata$column_count, self$ref$Header$column_count);
	  expect_equal(properties$metadata$col_count_p1, self$ref$Header$col_count_p1);
	  expect_equal(properties$metadata$col_count_p2, self$ref$Header$col_count_p2);
	  expect_equal(properties$metadata$mix_page_row_count, self$ref$Header$mix_page_row_count);
	  expect_equal(properties$metadata$lcs, self$ref$Header$lcs);
	  expect_equal(properties$metadata$lcp, self$ref$Header$lcp);

          self$columns = as.character(c(names(properties$metadata$columns)));
	  ref_columns = as.character(c(sapply(self$ref$Columns, FUN=function(x) { x$name; } )));
	  #print(self$columns);
	  #print(ref_columns);
	  expect_equal(self$columns, ref_columns);

	  ctype = unname(as.vector(sapply(properties$metadata$columns, FUN=function(x) { cf(x$type); })));
	  rtype = as.vector(sapply(self$ref$Columns, FUN=function(x) { cf(x$type); }));
	  #print(ctype);
	  #print(rtype);
	  expect_equal(ctype, rtype);
	  cformat = unname(as.vector(sapply(properties$metadata$columns, FUN=function(x) { x$format; })));
	  rformat = as.vector(sapply(self$ref$Columns, FUN=function(x) { x$format; }));
	  #print(cformat);
	  #print(rformat);
	  expect_equal(cformat, rformat);

	  self$columns_comparison = sapply(properties$metadata$columns, FUN=function(x) {
  	     function(x, y) {
	       #print(x); print(y);
	       if(is.null(x)) { expect_null(y); }
	       else if(is.na(x)) { expect_null(y); }
	       else { expect_equal(x, y); }
	     };
	  });
	  #print(self$columns_comparison);
	  
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
        ref = files[[file]]
        file=paste0("../", file);
	print(paste("Data sink", file));
      	sink = TestSinkData$new(ref);
        reader = CPPSAS7BDAT::Reader(file, sink);
	# Skip big5 and test16: json unicode cannot be decoded correctly 
	if(grepl("big5", file, fixed=TRUE) == FALSE &&
	   grepl("test16", file, fixed=TRUE) == FALSE) {
            reader$read_all();
	}
        rm(reader);
        rm(sink);
    }
})