read_sas <- function(filename)
{
  sink = SinkWholeData$new();
  reader <- CPPSAS7BDAT::Reader(filename, sink);
  reader$read_all();
  rm(reader);
  sink;
}