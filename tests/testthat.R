library(testthat)
library(CPPSAS7BDAT)

#options(testthat.output_file = "../reports/R-tests.xml")
test_check("CPPSAS7BDAT");  # reporter="junit"
#test_file("testthat/test_read_sas.R", package = "CPPSAS7BDAT")
#test_file("testthat/test_sink.R", package = "CPPSAS7BDAT")
#test_file("testthat/test_column_filter.R", package = "CPPSAS7BDAT")
#test_file("testthat/test_skip.R", package = "CPPSAS7BDAT")
