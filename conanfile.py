from conans import ConanFile

class CppSAS7BDATProject(ConanFile):
    name = "CppSAS7BDAT"
    version = "1.0"
    requires = (
        "catch2/2.13.6",
        "docopt.cpp/0.6.3",
        "fmt/8.0.1",
        "spdlog/1.9.2",
        "boost/1.71.0",
        "nlohmann_json/3.10.4"
    )
    generators = "cmake", "gcc", "txt", "cmake_find_package"

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

