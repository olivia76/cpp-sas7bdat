#!/bin/bash

echo "Creating default profile"
conan profile new default --detect
conan profile update settings.compiler.libcxx=libstdc++11 default
conan profile show default
