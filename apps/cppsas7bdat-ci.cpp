/**
 *  \file apps/cppsas7bdat-ci.cpp
 * 
 *  \brief Main file
 *
 *  \author Olivia Quinet
 */

#include <cppsas7bdat/version.hpp>
#include <cppsas7bdat/sas7bdat.hpp>
#include <cppsas7bdat/reader.hpp>
#include <boost/program_options.hpp>
//#include <boost/filesystem.hpp>
#include <fmt/core.h>
#include <iostream>

namespace 
{ 
  const int SUCCESS = 0; 
  const int ERROR_IN_COMMAND_LINE = 1; 
  const int ERROR_UNHANDLED_EXCEPTION = 2; 
  const int FAIL = -1;
} // namespace 

namespace {
  namespace po = boost::program_options; 

  /** 
   *  Define and parse the program options 
   */
  class PO {
  public:
    PO()
      : m_desc("Options")
    {
      m_desc.add_options()
        ("help,h", "Print help messages")
        ("version", "Version")
	("input,i", po::value<std::string>(), "Input")
        ;
      m_positionalOptions.add("input", -1);
    }

    bool parse(int argc, char* argv[])
    {
      po::store(po::command_line_parser(argc, argv).options(m_desc) 
                .positional(m_positionalOptions).run(), 
                m_vm);
      po::notify(m_vm);

      if ( m_vm.count("help")  ) return show_help();
      return true;
    }

    bool show_help()
    {
      std::cout << m_desc << std::endl;
      return false;
    }

    bool has(const std::string _label) const { return m_vm.count(_label); }
    template<typename _Tp> _Tp get(const std::string _label) const { return m_vm[_label].as<_Tp>(); }

    const po::variables_map& vm() const { return m_vm; }

  protected:
    po::options_description m_desc;
    po::positional_options_description m_positionalOptions; 
    po::variables_map m_vm;
  };
}

/**
 *  Get the version information and print it to stdout/export it
 *  to a json file
 */
void getVersion([[maybe_unused]] PO& po)
{
  fmt::print("Version: {}\n", cppsas7bdat::getVersion());
}

void processInput(PO& po)
{
  auto inputfilename = po.get<std::string>("input");
  fmt::print("Processing {}...\n", inputfilename);

  cppsas7bdat::Reader reader(inputfilename.c_str(), cppsas7bdat::reader::print());
  while(reader.read()) ;
}

int main(const int argc, char* argv[])
{
bool bOkay = true;
  try { 
    PO po;
    if(po.parse(argc, argv)) {
      if(po.has("version")) getVersion(po);
      if(po.has("input")) processInput(po);
    }
  }
  catch(boost::program_options::required_option& e) {
    fmt::print(stderr, "ERROR: {}\n", e.what());
    return ERROR_IN_COMMAND_LINE; 
  }
  catch(boost::program_options::error& e) { 
    fmt::print(stderr, "ERROR: {}\n", e.what());
    return ERROR_IN_COMMAND_LINE; 
  }
  catch(std::exception& e) { 
    fmt::print(stderr, "Unhandled Exception reached the top of main: {}, application will now exit\n", e.what());
    return ERROR_UNHANDLED_EXCEPTION; 
  }
  return bOkay ? SUCCESS : FAIL;
}
