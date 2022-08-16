#
# @brief: Wrapper for the C++ classes
# @author: Olivia Quinet
#

sas_reader <- function(filename, sink, include=NULL, exclude=NULL)
{
  r <- CPPSAS7BDAT::Reader(filename, sink, include=include, exclude=exclude);
  r;
}
