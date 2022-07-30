import os

from conans import ConanFile, CMake, tools


class CppDatasetTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake", "gcc", "txt", "cmake_find_package", "cmake_find_package_multi"
    build_policy = "missing"
    requires = (
        "cppsas7bdat/1.0",
        )

    def build(self):
        cmake = CMake(self)
        # Current dir is "test_package/build/<build_id>" and CMakeLists.txt is
        # in "test_package"
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy('*.so*', dst='bin', src='lib')
        self.copy('*.a*', dst='bin', src='lib')
        self.copy('*.h*', dst='include', src='include')

    def test(self):
        if not tools.cross_building(self):
            os.chdir("bin")
            self.run(".%sexample" % os.sep)
