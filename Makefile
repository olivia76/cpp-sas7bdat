all: build

.PHONY: build
build:
	cmake -S . -B ./build -DENABLE_CONAN:BOOL=ON; cmake --build ./build

.PHONY: build-tests
build-tests:
	mkdir -p build; cd build; cmake ..; cmake --build tests

.PHONY: tests
tests:
	make -C build test ARGS='-V'

.PHONY: build-debug
build-debug:
	mkdir -p build-debug; cd build-debug; cmake -DCMAKE_BUILD_TYPE=Debug ..; cmake --build .
