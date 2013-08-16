    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "tokens.hpp"


    //
#include "archive_init_list.hpp"

using namespace std;

void archive_init_list::set_archive_path(const std::string & val)
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

const vector<libdar::list_entry> archive_init_list::get_children_in_table(const std::string & dir) const
{
    if(ptr == NULL)
	throw WEBDAR_BUG;
    return ptr->get_children_in_table(dir);
}

bool archive_init_list::has_subdirectory(const std::string & dir) const
{
    if(ptr == NULL)
	throw WEBDAR_BUG;
    return ptr->has_subdirectory(dir);
}

void archive_init_list::inherited_run()
{
    if(ptr != NULL)
	throw WEBDAR_BUG;

    try
    {
	ptr = new (nothrow) libdar::archive(ui,
					    archpath,
					    basename,
					    EXTENSION,
					    read_opt);
	if(ptr == NULL)
	    throw exception_memory();

	try
	{
		// this is necessary before calling  get_children_in_table
	    ptr->init_catalogue(ui);
	}
	catch(...)
	{
	    delete ptr;
	    ptr = NULL;
	    throw;
	}
    }
    catch(libdar::Egeneric & e)
    {
	if(ptr != NULL)
	    throw WEBDAR_BUG;
	throw exception_libcall(e);
    }
}

