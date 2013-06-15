    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "tokens.hpp"


    //
#include "archive_test.hpp"

using namespace std;

void archive_test::set_archive_path(const std::string & val)
{
    try
    {
	archpath = libdar::path(val, true);
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }
}

void archive_test::inherited_run()
{
    try
    {
	libdar::archive arch = libdar::archive(ui,
					       archpath,
					       basename,
					       EXTENSION,
					       read_opt);

	libdar::statistics final = arch.op_test(ui,
						test_opt,
						progressive_report);
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }
}
