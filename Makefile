all: build

TESTS :=
OPTIONS :=

BUILD_TYPE := Release
PYTHON_VERSION := 3.8.12
VENV_NAME := $(shell cat .python-version)

.PHONY: build
build:
	cmake -S . -B ./build -DENABLE_CONAN:BOOL=ON -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE}
	cmake -S . -B ./build -DENABLE_CONAN:BOOL=ON -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE}
	cmake --build ./build --config ${BUILD_TYPE}

.PHONY: tests
tests:
	cd ./build; ctest -C ${BUILD_TYPE} --output-on-failure
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
	#	source $(PYENV_VIRTUAL_ENV)/bin/activate
	python3 python/setup.py develop

.PHONY: tests-python
tests-python:
	pip3 install -e ".[tests]"
	coverage run --source pycppsas7bdat -m py.test $(OPTIONS) $(TESTS) --junitxml=./pytest.xml
	coverage report --show-missing
	coverage html
