#include <cppsas7bdat/reader.hpp>
#include <cppsas7bdat/version.hpp>
#include <cppsas7bdat/datasource_ifstream.hpp>
#include <cppsas7bdat/datasink_null.hpp>

int main() {
  cppsas7bdat::getVersion();
  //auto reader = cppsas7bdat::Reader(cppsas7bdat::datasource::ifstream("file.sas7bdat"), cppsas7bdat::datasink::null());
  return 0;
}
