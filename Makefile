all: build

TESTS :=
OPTIONS :=

BUILD_TYPE := Release
PYTHON := python3.10
PYTHON_VERSION := 3.10.0  #3.8.12
VENV_NAME := $(shell cat .python-version)
ENABLE_CONAN := ON
ENABLE_COVERAGE := OFF
ENABLE_SANITIZER_ADDRESS := OFF
ENABLE_SANITIZER_LEAK := OFF
ENABLE_SANITIZER_UNDEFINED_BEHAVIOR := OFF
ENABLE_SANITIZER_THREAD := OFF
ENABLE_SANITIZER_MEMORY := OFF
ENABLE_R := OFF
ENABLE_TESTING := ON
ENABLE_PYTHON := OFF
PIP_OPTIONS := --user
CONAN_OPTIONS := -s:b compiler.cppstd=17 -s:h compiler.cppstd=17 --profile:build=default --build=missing

SHELL := /bin/bash

.PHONY: conan-package
conan-package:
	export CONAN_REVISIONS_ENABLED=1; export LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH; conan create . ${CONAN_OPTIONS}

.PHONY: configure
configure:
	mkdir -p build; cd build; conan install .. ${CONAN_OPTIONS} -o ENABLE_COVERAGE=${ENABLE_COVERAGE} -o ENABLE_R=${ENABLE_R} -o ENABLE_PYTHON=${ENABLE_PYTHON} -o ENABLE_TESTING=${ENABLE_TESTING}
	#mkdir -p build; cd build; conan install .. --build=missing -s compiler.cppstd=17 --profile:build=default -o ENABLE_COVERAGE=${ENABLE_COVERAGE} -o ENABLE_R=${ENABLE_R} -o ENABLE_PYTHON=${ENABLE_PYTHON} -o ENABLE_TESTING=${ENABLE_TESTING}
	#cmake -S . -B ./build -DENABLE_CONAN:BOOL=${ENABLE_CONAN} -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE} -DCMAKE_CXX_FLAGS="${CXX_FLAGS}" \
	#	-DENABLE_R:BOOL=${ENABLE_R} \
	#	-DENABLE_COVERAGE:BOOL=${ENABLE_COVERAGE} \
	#			-DENABLE_SANITIZER_ADDRESS:BOOL=${ENABLE_SANITIZER_ADDRESS} \
	#			-DENABLE_SANITIZER_LEAK:BOOL=${ENABLE_SANITIZER_LEAK} \
	#			-DENABLE_SANITIZER_UNDEFINED_BEHAVIOR:BOOL=${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR} \
	#			-DENABLE_SANITIZER_THREAD:BOOL=${ENABLE_SANITIZER_THREAD} \
	#			-DENABLE_SANITIZER_MEMORY:BOOL=${ENABLE_SANITIZER_MEMORY} \
	#			-DENABLE_TESTING:BOOL=${ENABLE_TESTING} \
	#			-DENABLE_PYTHON:BOOL=${ENABLE_PYTHON}

.PHONY: build
build: configure
	cd build; conan build .. ${CONAN_OPTIONS} -o ENABLE_COVERAGE=${ENABLE_COVERAGE} -o ENABLE_R=${ENABLE_R} -o ENABLE_PYTHON=${ENABLE_PYTHON} -o ENABLE_TESTING=${ENABLE_TESTING}

.PHONY: build-python
build-python:
	make -C . build ENABLE_PYTHON=ON

.PHONY: build-R
build-R:
	make -C R build

.PHONY: build-sanitizer
build-sanitizer:
	make -C . build BUILD_TYPE=RelWithDebInfo ENABLE_SANITIZER_ADDRESS=ON

.PHONY: test
test:
	source build/${BUILD_TYPE}/generators/conanrun.sh; cd test; ../build/${BUILD_TYPE}/test/tests ${TESTS}
	#cd ./build; ctest -C ${BUILD_TYPE} --output-on-failure ${TESTS}
	#make -C build test ARGS='-V'

.PHONY: build-debug
build-debug:
	mkdir -p build-debug; cd build-debug; cmake -DCMAKE_BUILD_TYPE=Debug ..; cmake --build .

.PHONY: pyenv pyenv-download pyenv-python pyenv-venv
pyenv: pyenv-download pyenv-python pyenv-venv

pyenv-download:
	curl -L https://github.com/pyenv/pyenv-installer/raw/master/bin/pyenv-installer | bash

pyenv-python:
	export PYTHON_CONFIGURE_OPTS="--enable-shared"; ~/.pyenv/bin/pyenv install --force $(PYTHON_VERSION)

pyenv-venv:
	~/.pyenv/bin/pyenv virtualenv -f --clear -p $(PYTHON) $(PYTHON_VERSION) $(VENV_NAME)

pyenv-activate:
	source $PYENV_VIRTUAL_ENV/bin/activate

pyenv-init:
	#source $(PYENV_VIRTUAL_ENV)/bin/activate
	#python3 setup.py develop
	pip3 install -e .

.PHONY: tests-python tests-python-install tests-python-run

tests-python-install:
	#pip3 install -e ".[tests]"
	source build/${BUILD_TYPE}/generators/conanrun.sh; pip3 install ".[tests]"

tests-python-run:
	coverage run --source pycppsas7bdat -m pytest $(OPTIONS) $(TESTS) --junitxml=./reports/pytest.xml

tests-python: tests-python-install tests-python-run
	coverage report --show-missing
	coverage html
	coverage xml -o build/coverage-python.xml

.PHONY: tests-R
tests-R: build-R
	cd test; Rscript testthat.R

.PHONY: lint
lint:
	yamllint -f colored .github

.PHONY: conan conan-install conan-setup
conan: conan-install conan-setup

conan-install:
	python3 -m pip install $(PIP_OPTIONS) --upgrade pip
	#pip3 install $(PIP_OPTIONS) --upgrade pip
	pip3 install $(PIP_OPTIONS) wheel setuptools gcovr==5.0 numpy cmaketools
	pip3 install $(PIP_OPTIONS) conan --upgrade

conan-profile:
	conan profile detect
	conan profile path default

.PHONY: benchmark
benchmark:
	make -C benchmark

.PHONY: clang-tidy
clang-tidy:
	clang-tidy -p build $(shell find src -type f -name *.cpp) include/cppsas7bdat/reader.hpp -extra-arg=-std=c++17  -checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus* -- -I include -I build

.PHONY: clean
clean:
	make -C build clean

.PHONY: very-clean
very-clean:
	rm -fr ./build ./test_package/build
