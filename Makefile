all: build

BUILD_TYPE := Release

.PHONY: build
build:
	cmake -S . -B ./build -DENABLE_CONAN:BOOL=ON -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE}
	cmake -S . -B ./build -DENABLE_CONAN:BOOL=ON -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE}
	cmake --build ./build --config ${BUILD_TYPE}

.PHONY: tests
tests:
	cd ./build; ctest -C ${BUILD_TYPE}
	#make -C build test ARGS='-V'

.PHONY: build-debug
build-debug:
	mkdir -p build-debug; cd build-debug; cmake -DCMAKE_BUILD_TYPE=Debug ..; cmake --build .
