#
# @brief: Wrapper for the C++ classes
# @author: Olivia Quinet
#

read_sas <- function(filename, include=NULL, exclude=NULL)
{
  sink <- SinkWholeData$new();
  r <- sas_reader(filename, sink, include=include, exclude=exclude);
  r$read_all();
  sink;
}
