SHELL := /bin/bash


TESTS :=
OPTIONS :=

PACKAGE_NAME := $(shell cat package.txt)
PACKAGE_VERSION := $(shell cat version.txt)

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

PIP_OPTIONS :=
#CONAN_OPTIONS := -s:b compiler.cppstd=17 -s:h compiler.cppstd=17 --profile:build=default --build=catch2/3.4.0 --build=missing
CONAN_OPTIONS := -s:b compiler.cppstd=20 -s:h compiler.cppstd=20 --profile:build=default --profile:host=default --build=missing

.PHONY: all
all: build

.PHONY: clean
clean:
	make -C build/${BUILD_TYPE} clean

.PHONY: very-clean
very-clean:
	rm -fr ./build ./test_package/build

.PHONY: configure
configure:
	mkdir -p build; cd build; conan install .. ${CONAN_OPTIONS} -o '&:ENABLE_COVERAGE=${ENABLE_COVERAGE}' -o '&:ENABLE_TESTING=${ENABLE_TESTING}' -o '(&:ENABLE_R=${ENABLE_R}' -o '&:ENABLE_PYTHON=${ENABLE_PYTHON}'

.PHONY: build
build: configure
	cd build; conan build .. ${CONAN_OPTIONS} -o '&:ENABLE_COVERAGE=${ENABLE_COVERAGE}' -o '&:ENABLE_TESTING=${ENABLE_TESTING}' -o '(&:ENABLE_R=${ENABLE_R}' -o '&:ENABLE_PYTHON=${ENABLE_PYTHON}'

.PHONY: tests
tests:
	source build/${BUILD_TYPE}/generators/conanrun.sh; cd tests; ../build/${BUILD_TYPE}/tests/tests ${TESTS} --skip-benchmarks

.PHONY: coverage
coverage:
	@gcovr --version
	@mkdir -p coverage
	cd build; gcovr --delete --print-summary --merge-mode-functions separate --exclude-unreachable-branches --html --html-details -o ../coverage/index.html --xml-pretty --xml ../coverage/coverage.xml --filter '../src/' --filter '../include/' --exclude '../test/'

.PHONY: conan-package
conan-package:
	export CONAN_REVISIONS_ENABLED=1; export LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH; conan create . ${CONAN_OPTIONS} --build-require #--format=json

clang-format:
	clang-format --verbose -i $(shell find include src tests test_package -name '*pp' -not -path 'test_package/build/*') --style=LLVM

.PHONY: pyenv pyenv-download pyenv-python pyenv-venv
pyenv: pyenv-download pyenv-python pyenv-venv

pyenv-download:
	curl -L https://github.com/pyenv/pyenv-installer/raw/master/bin/pyenv-installer | bash

pyenv-python:
	export PYTHON_CONFIGURE_OPTS="--enable-shared"; ~/.pyenv/bin/pyenv install --force $(PYTHON_VERSION)

pyenv-venv:
	~/.pyenv/bin/pyenv virtualenv $(PYTHON_VERSION) $(VENV_NAME)

.PHONY: pyenv-setup
pyenv-setup: pyenv-conan pyenv-tests

.PHONY: pyenv-conan
pyenv-conan:
	python3 -m pip install $(PIP_OPTIONS) --upgrade pip
	pip3 install $(PIP_OPTIONS) conan --upgrade
	pip3 install $(PIP_OPTIONS) wheel setuptools gcovr==5.0 'numpy<2.0.0' cmaketools

.PHONY: pyenv-tests
pyenv-tests:
	pip3 install $(PIP_OPTIONS) gcovr

.PHONY: codechecker
codechecker:
	rm -fr codechecker
	CodeChecker analyze build/Release/compile_commands.json -o ./codechecker || true
	CodeChecker parse ./codechecker -e html -o ./codechecker || true


.PHONY: build-python
build-python:
	make -C . build ENABLE_PYTHON=ON

.PHONY: build-R
build-R:
	make -C R build


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

conan-profile:
	conan profile detect
	conan profile path default

.PHONY: benchmark
benchmark:
	source build/${BUILD_TYPE}/generators/conanrun.sh; make -C benchmark
