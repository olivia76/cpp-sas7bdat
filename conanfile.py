from conans import ConanFile

class CppSAS7BDATProject(ConanFile):
    name = "CppSAS7BDAT"
    version = "1.0"
    requires = (
        "catch2/2.13.6",
        "docopt.cpp/0.6.2",
        "fmt/8.0.1",
        "spdlog/1.9.2",
        "boost/1.71.0",
        "nlohmann_json/3.10.4"
    )
    generators = "cmake", "gcc", "txt", "cmake_find_package"

    def requirements(self):
        pass

