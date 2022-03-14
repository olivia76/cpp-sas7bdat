#
# @author: Olivia Quinet
#
# @brief: Test the read_sas function
#

context("read_sas")
source("compare.R")

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

	   check_properties(properties, ref);

	   ref_idata = 1;
	   ref_irows = as.integer(names(ref$Data))
	   ref_irow = ref_irows[ref_idata];
	   
	   columns_comparison = get_columns_comparison(properties$columns);
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
	   expect_equal(ref_idata-1, length(ref$Data));
	   rm(sink);
	}
    }
})
