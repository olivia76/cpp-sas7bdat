#!/bin/bash

PROFILE="--profile package"
conan install . --install-folder=tmp/build --build=missing $PROFILE
conan source . --source-folder=tmp/source --install-folder=tmp/build
conan build . --source-folder=tmp/source --build-folder=tmp/build
conan package . --source-folder=tmp/source --build-folder=tmp/build --package-folder=tmp/package
conan export-pkg . cppsas7bdat/1.0@olivia.quinet/stable --force --source-folder=tmp/source --build-folder=tmp/build $PROFILE
conan test test_package cppsas7bdat/1.0@olivia.quinet/stable $PROFILE
