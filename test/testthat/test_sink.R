#
# @author: Olivia Quinet
#
# @brief: Test the sinks for R
#

context("Sink")

source("compare.R")

source("sink.R")
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
	    expect_equal(sink$ref_idata-1, length(ref$Data));
	}
        rm(reader);
        rm(sink);
    }
})


source("sink_chunk.R")
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
	    expect_equal(sink$ref_idata-1, length(ref$Data));
	}
        rm(reader);
        rm(sink);
    }
})


source("sink_data.R")
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
	    expect_equal(sink$ref_idata-1, length(ref$Data));
	}
        rm(reader);
        rm(sink);
    }
})