all: build

TESTS :=
OPTIONS :=

BUILD_TYPE := Release
PYTHON_VERSION := 3.8.12
VENV_NAME := $(shell cat .python-version)
ENABLE_CONAN := ON
ENABLE_COVERAGE := OFF
ENABLE_R := OFF
PIP_OPTIONS := --user

.PHONY: configure
configure:
	cmake -S . -B ./build -DENABLE_CONAN:BOOL=${ENABLE_CONAN} -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE} -DCMAKE_CXX_FLAGS="${CXX_FLAGS}" -DENABLE_R:BOOL=${ENABLE_R} -DENABLE_COVERAGE:BOOL=${ENABLE_COVERAGE}

.PHONY: build
build: configure
	cmake --build ./build --config ${BUILD_TYPE}

.PHONY: build-R
build-R:
	make -C R build

.PHONY: tests
tests:
	cd ./build; ctest -C ${BUILD_TYPE} --output-on-failure ${TESTS}
	#make -C build test ARGS='-V'

.PHONY: build-debug
build-debug:
	mkdir -p build-debug; cd build-debug; cmake -DCMAKE_BUILD_TYPE=Debug ..; cmake --build .

.PHONY: clean
clean:
	make -C build clean

.PHONY: pyenv pyenv-download pyenv-python pyenv-venv
pyenv: pyenv-download pyenv-python pyenv-venv

pyenv-download:
	curl -L https://github.com/pyenv/pyenv-installer/raw/master/bin/pyenv-installer | bash

pyenv-python:
	export PYTHON_CONFIGURE_OPTS="--enable-shared"; ~/.pyenv/bin/pyenv install --force $(PYTHON_VERSION)

pyenv-venv:
	~/.pyenv/bin/pyenv virtualenv -p python3.8 $(PYTHON_VERSION) $(VENV_NAME)

pyenv-activate:
	source $PYENV_VIRTUAL_ENV/bin/activate

pyenv-init:
	#source $(PYENV_VIRTUAL_ENV)/bin/activate
	python3 python/setup.py develop

.PHONY: tests-python tests-python-install tests-python-run

tests-python-install:
	pip3 install -e ".[tests]"

tests-python-run:
	coverage run --source pycppsas7bdat -m py.test $(OPTIONS) $(TESTS) --junitxml=./reports/pytest.xml

tests-python: tests-python-pip3 tests-python-run
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
	pip3 install $(PIP_OPTIONS) --upgrade pip
	pip3 install $(PIP_OPTIONS) wheel setuptools gcovr numpy cmaketools
	pip3 install $(PIP_OPTIONS) conan --upgrade

conan-setup:
	conan install conanfile.py

.PHONY: benchmark
benchmark:
	make -C benchmark
