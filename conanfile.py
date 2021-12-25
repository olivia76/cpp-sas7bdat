from conans import ConanFile


class CppStarterProject(ConanFile):
    name = "CppSAS7BDAT"
    version = "1.0"
    requires = (
        "catch2/2.13.6",
        "docopt.cpp/0.6.2",
        "fmt/8.0.1",
        "spdlog/1.9.2",
        "boost/1.71.0"
    )
    generators = "cmake", "gcc", "txt", "cmake_find_package"

    def requirements(self):
        pass

