from conan import ConanFile, tools
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout, CMake
import os

class CppSAS7BDATProject(ConanFile):
    name = "cppsas7bdat"
    version = "1.0.1"
    license = "apache"
    author="Olivia Quinet olivia.quinet@gmail.com"
    description = "A C++17 SAS7BDAT reader"
    url = "https://github.com/olivia76/cpp-sas7bdat"
    topics = ("c++17", "SAS7BDAT")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False], "ENABLE_COVERAGE": ["ON", "OFF"], "ENABLE_R": ["ON", "OFF"], "ENABLE_PYTHON": ["ON", "OFF"], "ENABLE_TESTING": ["ON", "OFF"]}
    default_options = {"shared": True, "fPIC": True, "ENABLE_COVERAGE": "OFF", "fmt/*:shared": False, "ENABLE_R": "OFF", "ENABLE_PYTHON": "OFF", "ENABLE_TESTING": "ON"}
    generators = "VirtualBuildEnv", "VirtualRunEnv"
    build_policy = "missing"
    requires = (
        "docopt.cpp/0.6.3",
        "fmt/8.0.1",
        "spdlog/1.9.2",
        #"boost/1.79.0",
        "nlohmann_json/3.10.4"
    )
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "apps/*", "test/*", "conanfile.py", "cmake/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.cache_variables["ENABLE_COVERAGE"] = self.options.ENABLE_COVERAGE
        tc.cache_variables["ENABLE_R"] = self.options.ENABLE_R
        tc.cache_variables["ENABLE_PYTHON"] = self.options.ENABLE_PYTHON
        tc.cache_variables["ENABLE_TESTING"] = self.options.ENABLE_TESTING
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def requirements(self):
        self.requires("boost/1.86.0", headers=True, libs=True, visible=True, transitive_headers=True, transitive_libs=True)
        self.test_requires("catch2/3.4.0")

    def build(self):
        cmake = CMake(self)
        cmake.verbose = True
        cmake.configure(variables={'CMAKE_EXPORT_COMPILE_COMMANDS': 'ON'})
        cmake.build()
        #cmake.test(target="test")
        #cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.verbose = True
        #cmake.configure()
        cmake.install()

    def package_info(self):
        # These are default values and doesn't need to be adjusted
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.bindirs = ["bin"]
        self.cpp_info.libs = ["cppsas7bdat"]

    def configure(self):
        self.options["boost"].without_python = False
        #self.options["boost"].python_executable = "python"
        #self.options["boost"].python_version = "3.8"
        #self.options["boost"].without_date_time = False
        #self.options["boost"].without_atomic = True
        #self.options["boost"].without_atomic = True
        #self.options["boost"].without_chrono = True
        #self.options["boost"].without_container = True
        #self.options["boost"].without_context = True
        #self.options["boost"].without_contract = True
        #self.options["boost"].without_coroutine = True
        #self.options["boost"].without_exception = True
        #self.options["boost"].without_fiber = True
        #self.options["boost"].without_filesystem = True
        #self.options["boost"].without_graph = True
        #self.options["boost"].without_iostreams = True
        #self.options["boost"].without_json = True
        #self.options["boost"].without_locale = True
        #self.options["boost"].without_log = True
        #self.options["boost"].without_math = True
        #self.options["boost"].without_mpi = True
        #self.options["boost"].without_nowide = True
        #self.options["boost"].without_program_options = True
        #self.options["boost"].without_random = True
        #self.options["boost"].without_regex = True
        #self.options["boost"].without_serialization = False
        #self.options["boost"].without_stacktrace = True
        #self.options["boost"].without_system = True
        #self.options["boost"].without_test = True
        #self.options["boost"].without_thread = True
        #self.options["boost"].without_timer = True
        #self.options["boost"].without_type_erasure = True
        #self.options["boost"].without_wave = True
        pass
