from cmaketools import setup
import os

install_requires = [
]

test_requires = [
    'pytest==3.7.1',
    'pytest-cov==2.3.1',
    'pandas'
]

def get_cmake_args():
    args = os.getenv('CMAKE_ARGS', "")
    return args.split(' ')

def get_version():
    return open("version.txt", "r").read()
    #with open("CMakeLists.txt", "r") as isf:
    #    for line in isf.readlines():
    #        i = line.find('VERSION')
    #        if i > 0:
    #            return line[i+7:].strip()

setup(
    name="pycppsas7bdat",
    version=get_version(),
    author="Olivia Quinet",
    author_email="olivia.quinet@gmail.com",
    description="A package to read SAS7BDAT file with C++17 and boost::python",
    url="https://github.com/olivia76/cpp-sas7bdat",
    license="Apache License 2.0",
    src_dir="python",
    ext_module_hint=r"Python3_add_library",
    has_package_data=True,
    install_requires=install_requires,
    extras_require={
          'tests': test_requires
    },
    configure_opts = ['-DENABLE_PYTHON:BOOL=ON', '-DENABLE_TESTING:BOOL=OFF', '-DENABLE_CONAN:BOOL=ON'] + get_cmake_args()
)
