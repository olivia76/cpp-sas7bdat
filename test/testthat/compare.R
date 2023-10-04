#
# @author: Olivia Quinet
#
# @brief: Test the sinks for R
#

library("RJSONIO")
files = RJSONIO::fromJSON("../files.json")

ts <- function(x) { substr(x, 0, 19); }
str <- function(x) { if(is.null(x)) { ""; } else { x; }; };
compression <- function(x) { if(is.null(x)) { "none"; } else if(x == "SASYZCRL") { "RLE"; } else { "RDC"; } };
cf <- function(x) { if(x == "string") { x; } else { "notstring"; } };

check_properties <- function(properties, ref) {
           expect_equal(properties$endianness, ref$Header$endianess);
	   expect_equal(properties$platform, ref$Header$platform);
	   expect_equal(ts(properties$date_created), ts(ref$Header$date_created));
	   expect_equal(ts(properties$date_modified), ts(ref$Header$date_modified));
	   expect_equal(properties$dataset_name, ref$Header$name);
	   #expect_equal(properties$encoding, ref$Header$encoding);
	   expect_equal(properties$file_type, ref$Header$file_type);
	   expect_equal(properties$sas_release, ref$Header$sas_release);
	   expect_equal(properties$sas_server_type, ref$Header$server_type);
	   expect_equal(properties$os_type, ref$Header$os_type);
	   expect_equal(properties$os_name, ref$Header$os_name);
	   expect_equal(properties$header_length, ref$Header$header_length);
	   expect_equal(properties$page_length, ref$Header$page_length);
	   expect_equal(properties$page_count, ref$Header$page_count);
	   expect_equal(properties$compression, compression(ref$Header$compression));
	   expect_equal(properties$creator, str(ref$Header$creator));
	   expect_equal(properties$creator_proc, str(ref$Header$creator_proc));
	   expect_equal(properties$row_length, ref$Header$row_length);
	   expect_equal(properties$row_count, ref$Header$row_count);
	   expect_equal(properties$column_count, ref$Header$column_count);
	   expect_equal(properties$col_count_p1, ref$Header$col_count_p1);
	   expect_equal(properties$col_count_p2, ref$Header$col_count_p2);
	   expect_equal(properties$mix_page_row_count, ref$Header$mix_page_row_count);
	   expect_equal(properties$lcs, ref$Header$lcs);
	   expect_equal(properties$lcp, ref$Header$lcp);

           columns = as.character(c(names(properties$columns)));
	   ref_columns = as.character(c(sapply(ref$Columns, FUN=function(x) { x$name; } )));
	   #print(columns);
	   #print(ref_columns);
	   expect_equal(columns, ref_columns);

	   ctype = unname(as.vector(sapply(properties$columns, FUN=function(x) { cf(x$type); })));
	   rtype = as.vector(sapply(ref$Columns, FUN=function(x) { cf(x$type); }));
	   #print(ctype);
	   #print(rtype);
	   expect_equal(ctype, rtype);
	   cformat = unname(as.vector(sapply(properties$columns, FUN=function(x) { x$format; })));
	   rformat = as.vector(sapply(ref$Columns, FUN=function(x) { x$format; }));
	   #print(cformat);
	   #print(rformat);
	   expect_equal(cformat, rformat);
}

get_columns_comparison <- function(columns) {
           comp_default <- function(x, y) {
	       #print(x); print(y);
	       if(is.null(x)) { expect_null(y); }
	       else if(is.na(x)) { expect_null(y); }
	       else { expect_equal(x, y); }
	   };
           comp_datetime <- function(x, y) {
	       #print(x); print(y);
	       if(is.null(x)) { expect_null(y); }
	       else if(is.na(x)) { expect_null(y); }
	       else { expect_equal(ts(x), ts(y)); }
	   };

           sapply(columns, FUN=function(x) {
	     if(x$type == "datetime") { comp_datetime; } else { comp_default; }
	   });
}
