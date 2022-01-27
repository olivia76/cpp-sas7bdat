require(CPPSAS7BDAT)
library(R6)
library(rbenchmark)

print(CPPSAS7BDAT::getVersion());

MySink <- R6Class("MySink",
     public=list(
	initialize = function() {
	#	    print("MySink");
	},
	set_properties = function(properties) {
	},
	push_row = function(irow, row) {
	}
     )
);

MySinkChunk <- R6Class("MySinkChunk",
     public=list(
	chunk_size = NULL,
	initialize = function(chunk_size=10000) {
    	    self$chunk_size = chunk_size;
	#    print("MySinkChunk");
	},
	set_properties = function(properties) {
	},
	push_rows = function(istartrow, iendrow, rows) {
	#	print(rows);
	}
     )
);

fct1 <- function(sink) {
     #sink <- MySink$new();
     #sink <- MySinkChunk$new(10000);
     #r <- CPPSAS7BDAT::Reader("../test/data_AHS2013/topical.sas7bdat", sink);
     r <- CPPSAS7BDAT::Reader("../test/data_misc/numeric_1000000_2.sas7bdat", sink);
     r$read_all();
}



benchmark("Sink" = {
		 print("Sink")
		 sink <- MySink$new();
		 fct1(sink)
		 },
          "SinkChunk" = {
		 print("SinkChunk")
		 sink <- MySinkChunk$new(10000);
		 fct1(sink)
		 },
          replications=10,
	  columns = c("test", "replications", "elapsed",
                      "relative", "user.self", "sys.self"))