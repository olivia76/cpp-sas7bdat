#!/bin/bash

echo "Creating package profile"
conan profile new package --detect
conan profile update settings.compiler.libcxx=libstdc++11 package
conan profile show package
