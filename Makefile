all: build

TESTS :=
OPTIONS :=

BUILD_TYPE := Release
PYTHON_VERSION := 3.8.12
VENV_NAME := $(shell cat .python-version)

ENABLE_R := OFF

.PHONY: build
build:
	cmake -S . -B ./build -DENABLE_CONAN:BOOL=ON -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE} -DCMAKE_CXX_FLAGS="${CXX_FLAGS}" -DENABLE_R:BOOL=${ENABLE_R}
	cmake -S . -B ./build -DENABLE_CONAN:BOOL=ON -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE} -DCMAKE_CXX_FLAGS="${CXX_FLAGS}" -DENABLE_R:BOOL=${ENABLE_R}
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
	#export CMAKE_ARGS="-DCMAKE_BUILD_TYPE:STRING=RelWithDebInfo -DENABLE_COVERAGE:BOOL=ON"; pip3 install -e ".[tests]"
	pip3 install -e ".[tests]"
	coverage run --source pycppsas7bdat -m py.test $(OPTIONS) $(TESTS) --junitxml=./reports/pytest.xml
	coverage report --show-missing
	coverage html	
	coverage xml -o build/coverage-python.xml

.PHONY: lint
lint:
	yamllint -f colored .github/workflows/

.PHONY: conan conan-install conan-setup
conan: conan-install conan-setup

conan-install:
	pip install conan

conan-setup:
	conan install conanfile.py

.PHONY: benchmark
benchmark:
	make -C benchmark
