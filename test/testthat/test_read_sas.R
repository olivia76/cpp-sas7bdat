#
# @author: Olivia Quinet
#
# @brief: Test the read_sas function
#

context("read_sas")

library("RJSONIO")

files = RJSONIO::fromJSON("../files.json")

ts <- function(x) { substr(x, 0, 19); }
str <- function(x) { if(is.null(x)) { ""; } else { x; }; };
compression <- function(x) { if(is.null(x)) { "none"; } else if(x == "SASYZCRL") { "RLE"; } else { "RDC"; } };
cf <- function(x) { if(x == "string") { x; } else { "notstring"; } };

test_that("I can read a SAS7BDAT file with the read_sas function", {
    for(file in names(files)) {
    #file="data_AHS2013/homimp.sas7bdat"; {
        ref = files[[file]]
        file=paste0("../", file);
	print(paste("read_sas:", file));
	#print(ref)
	# Skip big5 and test16: json unicode cannot be decoded correctly 
	if(grepl("big5", file, fixed=TRUE) == FALSE &&
	   grepl("test16", file, fixed=TRUE) == FALSE) {
	   sink <- CPPSAS7BDAT::read_sas(file);
           properties <- sink$properties;
	   expect_equal(properties$header$endianness, ref$Header$endianess);
	   expect_equal(properties$header$platform, ref$Header$platform);
	   expect_equal(ts(properties$header$date_created), ts(ref$Header$date_created));
	   expect_equal(ts(properties$header$date_modified), ts(ref$Header$date_modified));
	   expect_equal(properties$header$dataset_name, ref$Header$name);
	   #expect_equal(properties$header$encoding, ref$Header$encoding);
	   expect_equal(properties$header$file_type, ref$Header$file_type);
	   expect_equal(properties$header$sas_release, ref$Header$sas_release);
	   expect_equal(properties$header$sas_server_type, ref$Header$server_type);
	   expect_equal(properties$header$os_type, ref$Header$os_type);
	   expect_equal(properties$header$os_name, ref$Header$os_name);
	   expect_equal(properties$header$header_length, ref$Header$header_length);
	   expect_equal(properties$header$page_length, ref$Header$page_length);
	   expect_equal(properties$header$page_count, ref$Header$page_count);
	   expect_equal(properties$metadata$compression, compression(ref$Header$compression));
	   expect_equal(properties$metadata$creator, str(ref$Header$creator));
	   expect_equal(properties$metadata$creator_proc, str(ref$Header$creator_proc));
	   expect_equal(properties$metadata$row_length, ref$Header$row_length);
	   expect_equal(properties$metadata$row_count, ref$Header$row_count);
	   expect_equal(properties$metadata$column_count, ref$Header$column_count);
	   expect_equal(properties$metadata$col_count_p1, ref$Header$col_count_p1);
	   expect_equal(properties$metadata$col_count_p2, ref$Header$col_count_p2);
	   expect_equal(properties$metadata$mix_page_row_count, ref$Header$mix_page_row_count);
	   expect_equal(properties$metadata$lcs, ref$Header$lcs);
	   expect_equal(properties$metadata$lcp, ref$Header$lcp);
	   
	   ref_idata = 1;
	   ref_irows = as.integer(names(ref$Data))
	   ref_irow = ref_irows[ref_idata];
	   
	   columns_comparison = sapply(properties$metadata$columns, FUN=function(x) {
  	     function(x, y) {
	       #print(x); print(y);
	       if(is.null(x)) { expect_null(y); }
	       else if(is.na(x)) { expect_null(y); }
	       else { expect_equal(x, y); }
	     };
	   });
	   columns = names(columns_comparison);
	   df <- sink$df;

	   for(irow in seq(0, ref$Header$row_count-1)) {
	     if(irow == ref_irow) {
    	       #print(irow);
	       ref_row = as.list(ref$Data[[ref_idata]]);
	       names(ref_row) = columns;
	       #print(ref_row);

	       for(icol in seq_along(columns_comparison)) {
	         colname = columns[[icol]]
	      	 col = unname(unlist(df[colname]))
		 #print(icol)
		 #print(colname)
	         #print(col)
    	         columns_comparison[[icol]](col[[irow+1]], ref_row[[icol]]);
	       }

	       ref_idata = ref_idata+1;
	       ref_irow = ref_irows[ref_idata];
	     }
	   }
	   rm(sink);
	}
    }
})
