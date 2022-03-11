require(CPPSAS7BDAT)
library(R6)
library(rbenchmark)

print(CPPSAS7BDAT::getVersion());

args <- commandArgs(trailingOnly = TRUE)

fct1 <- function(sink) {
     r <- CPPSAS7BDAT::sas_reader(args[1], sink);
     r$read_all();
}

benchmark(#"Sink" = {
	  #	 print("Sink")
	  #	 sink <- SinkByRow$new();
	  #	 fct1(sink)
	  #	 #print(sink$df)
	  #	 },
          "SinkChunk" = {
		 print("SinkByChunk")
		 sink <- SinkByChunk$new(10000);
		 fct1(sink)
		 #print(sink$df)
		 },
          "SinkData" = {
		 print("SinkWholeData")
		 sink <- SinkWholeData$new();
		 fct1(sink)
		 #print(sink$df)
		 },
          "read_sas" = {
	         print("read_sas")
		 sink <- read_sas(args[1])
		 #print(sink$df)
	         },
          replications=1,
	  columns = c("test", "replications", "elapsed",
                      "relative", "user.self", "sys.self"))