# cpp-sas7bdat

## Summary
This is a toy project with cmake and C++ external polymorphism.

This project is a C++17 implementation of a SAS7BDAT file reader based
on different existing projects:

- https://github.com/WizardMac/ReadStat/
- https://github.com/tk3369/SASLib.jl
- https://pypi.org/project/sas7bdat/

The C++ external polymorphism design is used internally at 2 levels:

- For the dataset's columns
- For the datasink

### Datset's columns

Each column has a specific type and conversion/format operators.  The
external polymorphism is used internally to store the exact operator,
including endianness, 32/64 bits, ...

Supported types:
- string (`std::string`)
- integer (`long`)
- number (`double`)
- datetime (`boost::posix_time::ptime`)
- date (`boost::gregorian::date`)
- time (`boost::posix_time::time_duration`)

Each formatter [class](src/formatters.hpp) implements one or several *getters* as well as
the *to_string* method. 

### Datasink

2 simple datasinks are provided in this package:
[print](include/cppsas7bdat/datasink_print.hpp) and
[csv](include/cppsas7bdat/datasink_csv.hpp).

The first one directly prints the content of the file (header and
data) to the screen and the second one is a very basic csv writer (no
field protection beside the double quotes, no encoding, ...).


## Usage 

```c++
// See for example apps/cppsas7bdat-ci.cpp

#include <cppsas7bdat/sas7bdat.hpp>

struct MyDataSink {
	MyDataSink(...) { /* ... */ }
	
	/// This method is called once the header/metadata is read.
	void set_properties(const cppsas7bdat::Properties& _properties) { /* ... */ }
	/// This method is called for each new row.
	void read_row(const size_t _irow, cppsas7bdat::Column::PBUF _p) { /* ... */ }
};

void read_file(const char* _filename)
{
	cppsas7bdat::Reader reader(_filename, MyDataSink(...));
	
	// Read row by row
	while(reader.read_row());
	
	// OR read the whole file
	reader.read();
}

```


## cmake

Inspired from https://github.com/cpp-best-practices/cpp_starter_project

## C++ external polymorphism

This design pattern is very nicely explained in a talk of Klaus Iglberger - Breaking Dependencies: Type Erasure - A Design Analysis (https://www.youtube.com/watch?v=7GIz9SmRgyc)


## boost

## Catch2
Catch2 needs to be installed in the system

```bash
git clone https://github.com/catchorg/Catch2 --branch v2.x
cd Catch2
mkdir build; cd build; cmake ..; make; sudo make install
```

## fmt
fmt needs to be install in the system

```bash
git clone https://github.com/fmtlib/fmt.git
cd fmt
mkdir build; cd build; cmake ..; make; sudo make install
```

## sdplog
spdlog needs to be install in the system

```bash
git clone https://github.com/gabime/spdlog.git
cd spdlog
mkdir build; cd build; cmake ..; make; sudo make install
```

## docopt

```bash
git clone git@github.com:docopt/docopt.cpp.git
cd docopt.cpp
mkdir build; cd build; cmake ..; make; sudo make install
```

## json

```bash
git clone git@github.com:nlohmann/json.git
cd json
mkdir build; cd build; cmake ..; make; sudo make install
```
   
