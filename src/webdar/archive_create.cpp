    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers

    //
#include "archive_create.hpp"

using namespace std;

void archive_create::set_archive_path(const string & val)
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

void archive_create::set_archive_options_reference(const string & refpath,
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

void archive_create::set_fs_root(const string & val)
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

void archive_create::inherited_run()
{
    try
    {
	libdar::archive *ref = NULL;

	try
	{
		// in case of differential / incremental backup
		// we must open the archive of reference
		// and obtain an libdar::archive object to
		// be added to the options passed to the create
		// constructor

	    if(has_ref)
	    {
		ref = new (nothrow) libdar::archive(ui,
						    libdar::path(ref_path),
						    ref_basename,
						    ref_extension,
						    ref_opt);
		if(ref == NULL)
		    throw exception_memory();
		opt.set_reference(ref);
	    }

		// now we can create the archive

	    libdar::archive target = libdar::archive(ui,
						     fs_root,
						     archpath,
						     basename,
						     extension,
						     opt,
						     progressive_report);

		// as the object being local to the local block
		// it will be destroyed automatically (and the archive
		// will be closed) once we will have exit this local block
	}
	catch(...)
	{
	    if(ref != NULL)
		delete ref;
	    opt.set_reference(NULL);
	    throw;
	}
	if(ref != NULL)
	    delete ref;
	opt.set_reference(NULL);
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }
}

