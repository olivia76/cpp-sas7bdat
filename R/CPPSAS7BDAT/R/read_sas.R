read_sas <- function(filename)
{
  sink = SinkData$new();
  reader <- CPPSAS7BDAT::Reader(filename, sink);
  reader$read_all();
  sink;
}