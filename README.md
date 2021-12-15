# cpp-sas7bdat

## Summary
This is a toy project with cmake and C++ external polymorphism.

This project is a C++17 implementation of a SAS7BDAT file reader based
on different existing projects:

- https://github.com/tk3369/SASLib.jl
- https://pypi.org/project/sas7bdat/



## cmake

Inspired from https://github.com/cpp-best-practices/cpp_starter_project

## C++ external polymorphism

This design pattern is very nicely explained in a talk of Klaus Iglberger - Breaking Dependencies: Type Erasure - A Design Analysis (https://www.youtube.com/watch?v=7GIz9SmRgyc)


## boost

## Catch2
Catch2 needs to be installed in the system

    git clone https://github.com/catchorg/Catch2 --branch v2.x
    cd Catch2
    mkdir build; cd build; cmake ..; make; sudo make install

## fmt
fmt needs to be install in the system

    git clone https://github.com/fmtlib/fmt.git
    cd fmt
    mkdir build
    cd build
    cmake ..
    make
    sudo make install

## sdplog
spdlog needs to be install in the system

    git clone https://github.com/gabime/spdlog.git
    cd spdlog
    mkdir build
    cd build
    cmake ..
    make
    sudo make install

## docopt

    git clone git@github.com:docopt/docopt.cpp.git
    cd docopt.cpp
    mkdir build
    cd build
    cmake ..
    make
    sudo make install

## json

    git clone git@github.com:nlohmann/json.git
    cd json
    mkdir build
    cd build
    cmake ..
    make
    sudo make install
   
