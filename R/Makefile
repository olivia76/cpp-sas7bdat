.PHONY: setup
setup:
	sudo apt-get -y install libxml2-dev libfontconfig-dev libfribidi-dev libharfbuzz-dev libfreetype6-dev libpng-dev libtiff5-dev libjpeg-dev
	Rscript -e 'install.packages(c("devtools","Rcpp"),repos="https://cran.rstudio.com/",dependencies=TRUE);'

.PHONY: build
build:
	./build.bash

clean:
	make -C CPPSAS7BDAT/src clean
