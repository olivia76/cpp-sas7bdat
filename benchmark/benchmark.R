require(CPPSAS7BDAT)
library(R6)
library(rbenchmark)

print(CPPSAS7BDAT::getVersion());

args <- commandArgs(trailingOnly = TRUE)

fct1 <- function(sink) {
     r <- CPPSAS7BDAT::Reader(args[1], sink);
     r$read_all();
}

benchmark(#"Sink" = {
	#	 print("Sink")
	#	 sink <- Sink$new();
	#	 fct1(sink)
	#	 #print(sink$df)
	#	 },
          "SinkChunk" = {
		 print("SinkChunk")
		 sink <- SinkChunk$new(10000);
		 fct1(sink)
		 #print(sink$df)
		 },
          "SinkData" = {
		 print("SinkData")
		 sink <- SinkData$new();
		 fct1(sink)
		 #print(sink$df)
		 },
          replications=1,
	  columns = c("test", "replications", "elapsed",
                      "relative", "user.self", "sys.self"))