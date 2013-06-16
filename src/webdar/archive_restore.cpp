    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "tokens.hpp"

    //
#include "archive_restore.hpp"

using namespace std;


archive_restore::archive_restore():
    archpath("/"),
    fs_root("/")
{
    progressive_report = NULL;
}

void archive_restore::set_archive_path(const std::string & val)
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

void archive_restore::set_fs_root(const std::string & val)
{
    try
    {
	fs_root = libdar::path(val, true);
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }
}

void archive_restore::inherited_run()
{
    try
    {
	libdar::archive arch = libdar::archive(ui,
					       archpath,
					       basename,
					       EXTENSION,
					       read_opt);

	libdar::statistics final = arch.op_extract(ui,
						   fs_root,
						   extract_opt,
						   progressive_report);
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }
}
