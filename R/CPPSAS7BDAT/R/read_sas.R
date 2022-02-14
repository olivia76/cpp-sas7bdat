read_sas <- function(filename, include=NULL, exclude=NULL)
{
  sink <- SinkWholeData$new();
  r <- CPPSAS7BDAT::Reader(filename, sink, include=include, exclude=exclude);
  r$read_all();
  sink;
}