.PHONY: setup
setup:
	sudo apt-get -y install libxml2-dev libfontconfig-dev libfribidi-dev libharfbuzz-dev libfreetype6-dev libpng-dev libtiff5-dev libjpeg-dev
	Rscript -e 'install.packages(c("devtools","Rcpp"),repos="https://cran.rstudio.com/",dependencies=TRUE);'

BUILD_TYPE := Release

ifndef R_HOME
R_HOME := $(shell R RHOME | tail -1)
endif

export BLAS_LIBS
export R_XTRA_CPPFLAGS
export R_LIBS
export R_HOME

RCPP_INC := -I $(shell $(R_HOME)/bin/Rscript -e 'cat(R.home("include"))' | tail -1) $(shell $(R_HOME)/bin/Rscript -e 'Rcpp:::CxxFlags()' | tail -1)
RCPP_LIB := $(shell $(R_HOME)/bin/R CMD config BLAS_LIBS | tail -1) $(shell $(R_HOME)/bin/R CMD config LAPACK_LIBS | tail -1)
RINSIDE_INC := $(shell $(R_HOME)/bin/Rscript -e 'RInside:::CxxFlags()' | tail -1) 
RINSIDE_LIB := $(shell $(R_HOME)/bin/Rscript -e 'RInside:::LdFlags()' | tail -1)

export RCPP_INC
export RCPP_LIB
export RINSIDE_INC
export RINSIDE_LIB

CXX_FLAGS := $(RCPP_INC) $(RCPP_LIB) $(RINSIDE_INC) $(RINSIDE_LIB) $(R_XTRA_CPPFLAGS) #$(R_LIBS)
export CXX_FLAGS

.PHONY: build
build:
	make -C .. ENABLE_R=ON ENABLE_TESTING=OFF ENABLE_PYTHON=OFF
	./build.bash

clean:
	make -C CPPSAS7BDAT/src clean
