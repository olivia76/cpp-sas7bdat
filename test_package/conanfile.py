import os

from conans import ConanFile, tools
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout, CMake

class CppDatasetTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain", "VirtualBuildEnv", "VirtualRunEnv"
    build_policy = "missing"
    requires = (
        "cppsas7bdat/1.0.1",
        )

    def build(self):
        cmake = CMake(self)
        cmake.verbose = True
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy('*.so*', dst='bin', src='lib')
        self.copy('*.a*', dst='bin', src='lib')
        self.copy('*.h*', dst='include', src='include')

    def test(self):
        if not tools.cross_building(self):
            self.run(os.path.sep.join([".", "example"]), env="conanrun", run_environment=True)
