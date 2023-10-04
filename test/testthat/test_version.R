#
# @author: Olivia Quinet
#
# @brief: Test the version
#

context("Version")

test_that("I can get the version of the package", {
   expect_equal(getVersion(), "1.0.1");
})
