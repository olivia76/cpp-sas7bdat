#
# @author: Olivia Quinet
#
# @brief: Test the include/exclude column filter
#

context("column filter")

test_that("I can specify columns that I want to include/exclude when reading a SAS7BDAT file", {
    file="data/file2.sas7bdat"; {
        file=paste0("../", file);
	print(paste("include/exclude:", file));
	# Include
	sink <- read_sas(file, include=c("q1"));
        properties <- sink$properties;
	columns <- properties$columns;
	expect_equal( names(columns), c("q1"));
	# Exclude
	sink <- read_sas(file, exclude=c("q1"));
        properties <- sink$properties;
	columns <- properties$columns;
	expect_equal( names(columns), c("c1", "c2", "q2"));
	# Bad include
	x = try(sink <- read_sas(file, include=c(1, 2)));
	expect_equal(class(x), "try-error")
	# Bad exclude
	x = try(sink <- read_sas(file, exclude=c(1, 2)));
	expect_equal(class(x), "try-error")
    }
})
