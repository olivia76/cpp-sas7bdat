#
# @author: Olivia Quinet
#
# @brief: Test the sinks for R
#

context("Sink/skip")

source("compare.R")

reader_skip <- function(reader, ref) {
     for(irefline in names(ref$Data)) {
        irefline = as.integer(irefline);
	#print(paste("reader_skip:", irefline));
	expect_equal(reader$skip(irefline - reader$current_row_index()), TRUE);
	expect_equal(reader$read_row(), TRUE);
     }
     reader$end_of_data();
}

source("sink.R")
test_that("I can skip a SAS7BDAT file with the default Sink", {
    #skip("skip");
    for(file in names(files)) {
    #file="data_pandas/datetime.sas7bdat"; {
    #file="../data_AHS2013/omov.sas7bdat"; {
        ref = files[[file]]
        file=paste0("../", file);
	print(paste("Default sink:", file));
      	sink = TestSink$new(ref);
        reader <- CPPSAS7BDAT::sas_reader(file, sink);
	# Skip big5 and test16: json unicode cannot be decoded correctly 
	if(grepl("big5", file, fixed=TRUE) == FALSE &&
	   grepl("test16", file, fixed=TRUE) == FALSE) {
	   reader_skip(reader, ref);
	   expect_equal(sink$ref_idata-1, length(ref$Data));
	}
        rm(reader);
        rm(sink);
    }
})


source("sink_chunk.R")
test_that("I can read a SAS7BDAT file with the Chunk Sink", {
    #skip("skip");
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
	    reader_skip(reader, ref);
	    expect_equal(sink$ref_idata-1, length(ref$Data));
	}
        rm(reader);
        rm(sink);
    }
})


source("sink_data.R")
test_that("I can read a SAS7BDAT file with the Data Sink", {
    #skip("skip");
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
	    reader_skip(reader, ref);
	    expect_equal(sink$ref_idata-1, length(ref$Data));
	}
        rm(reader);
        rm(sink);
    }
})