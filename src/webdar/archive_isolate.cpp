    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers

    //
#include "archive_isolate.hpp"

using namespace std;

void archive_isolate::set_archive_path(const string & val)
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

void archive_isolate::set_archive_reference(const string & refpath,
					    const string & basename,
					    const string & extension,
					    const libdar::archive_options_read & readopt)
{
    has_ref = true;
    ref_path = refpath;
    ref_basename = basename;
    ref_extension = extension;
    ref_opt = readopt;
}

void archive_isolate::inherited_run()
{
    try
    {
	libdar::archive *ref = NULL;

	try
	{
		// we must open the archive of reference
		// and obtain an libdar::archive object to
		// be added to the options passed to the isolate
		// constructor

	    if(!has_ref)
		throw WEBDAR_BUG;
	    else
	    {
		ref = new (nothrow) libdar::archive(ui,
						    libdar::path(ref_path),
						    ref_basename,
						    ref_extension,
						    ref_opt);
		if(ref == NULL)
		    throw exception_memory();
	    }

		// now we can isolate the archive

	    libdar::archive target = libdar::archive(ui,
						     archpath,
						     ref,
						     basename,
						     extension,
						     opt);

		// as the object being local to the local block
		// it will be destroyed automatically (and the archive
		// will be closed) once we will have exit this local block
	}
	catch(...)
	{
	    if(ref != NULL)
		delete ref;
	    throw;
	}
	if(ref != NULL)
	    delete ref;
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }
}

