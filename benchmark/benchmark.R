require(CPPSAS7BDAT)
library(R6)
library(rbenchmark)

print(CPPSAS7BDAT::getVersion());

args <- commandArgs(trailingOnly = TRUE)

MySink <- R6Class("MySink",
     public=list(
	initialize = function() {
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
	},
	set_properties = function(properties) {
	},
	push_rows = function(istartrow, iendrow, rows) {
	}
     )
);

fct1 <- function(sink) {
     r <- CPPSAS7BDAT::Reader(args[1], sink);
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