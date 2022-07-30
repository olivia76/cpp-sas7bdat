from conans import ConanFile, tools, CMake
from conan.tools.cmake import CMakeToolchain, cmake_layout

class CppSAS7BDATProject(ConanFile):
    name = "cppsas7bdat"
    version = "1.0"
    license = "apache"
    author="Olivia Quinet olivia.quinet@gmail.com"
    description = "A C++17 SAS7BDAT reader"
    url = "https://github.com/olivia76/cpp-sas7bdat"
    topics = ("c++17", "SAS7BDAT")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True, "fmt:shared": False}
    generators = "cmake", "gcc", "txt", "cmake_find_package", "cmake_find_package_multi"
    build_policy = "missing"
    requires = (
        "catch2/2.13.6",
        "docopt.cpp/0.6.3",
        "fmt/8.0.1",
        "spdlog/1.9.2",
        "boost/1.71.0",
        "nlohmann_json/3.10.4"
    )
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "apps/*", "test/*", "conanfile.py", "cmake/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def package(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()
        
    def package_info(self):
        # These are default values and doesn't need to be adjusted
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.bindirs = ["bin"]

        self.cpp_info.libs = ["cppsas7bdat"]


    def configure(self):
        self.options["boost"].without_python = False
        self.options["boost"].without_date_time = False
        self.options["boost"].without_atomic = True
        self.options["boost"].without_atomic = True
        self.options["boost"].without_chrono = True
        self.options["boost"].without_container = True
        self.options["boost"].without_context = True
        self.options["boost"].without_contract = True
        self.options["boost"].without_coroutine = True
        self.options["boost"].without_exception = True
        self.options["boost"].without_fiber = True
        self.options["boost"].without_filesystem = True
        self.options["boost"].without_graph = True
        self.options["boost"].without_iostreams = True
        #self.options["boost"].without_json = True
        self.options["boost"].without_locale = True
        self.options["boost"].without_log = True
        self.options["boost"].without_math = True
        self.options["boost"].without_mpi = True
        #self.options["boost"].without_nowide = True
        self.options["boost"].without_program_options = True
        self.options["boost"].without_random = True
        self.options["boost"].without_regex = True
        self.options["boost"].without_serialization = False
        self.options["boost"].without_stacktrace = True
        self.options["boost"].without_system = True
        self.options["boost"].without_test = True
        self.options["boost"].without_thread = True
        self.options["boost"].without_timer = True
        self.options["boost"].without_type_erasure = True
        self.options["boost"].without_wave = True
    
    def requirements(self):
        pass

