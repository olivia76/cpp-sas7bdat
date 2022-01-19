from cmaketools import setup

install_requires = [
]

test_requires = [
    'pytest==3.7.1',
    'pytest-cov==2.3.1',
    'pandas'
]

setup(
    name="pycppsas7bdat",
    version="0.0.1",
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
    configure_opts = ['-DENABLE_CONAN:BOOL=ON']  #['-DBOOST_ROOT=C:\\Users\\..\\AppData\\Local\\Programs\\boost_1_71_0']
)