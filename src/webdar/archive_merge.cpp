    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers

    //
#include "archive_merge.hpp"

using namespace std;

void archive_merge::set_archive_path(const string & val)
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

void archive_merge::set_archive_reference(const string & refpath,
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

void archive_merge::set_archive_options_auxilliary(const string & refpath,
						   const string & basename,
						   const string & extension,
						   const libdar::archive_options_read & readopt)
{
    has_aux = true;
    aux_path = refpath;
    aux_basename = basename;
    aux_extension = extension;
    aux_opt = readopt;
}

void archive_merge::inherited_run()
{
    try
    {
	libdar::archive *ref = NULL;
	libdar::archive *aux = NULL;

	try
	{
		// we must open the archive of reference
		// and obtain an libdar::archive object to
		// be added to the options passed to the merge
		// constructor

	    if(!has_ref)
		throw WEBDAR_BUG; // ref is mandatory for merging
	    else
	    {
		ui.warning("--- Opening the archive of reference...");
		ref = new (nothrow) libdar::archive(ui,
						    libdar::path(ref_path),
						    ref_basename,
						    ref_extension,
						    ref_opt);
		if(ref == NULL)
		    throw exception_memory();
		ui.warning("--- The archive of reference is now opened");
		if(has_aux)
		{
		    ui.warning("--- Opening the auxilliary archive of reference...");
		    aux = new (nothrow) libdar::archive(ui,
							libdar::path(aux_path),
							aux_basename,
							aux_extension,
							aux_opt);
		    if(aux == NULL)
			throw exception_memory();
		    opt.set_auxilliary_ref(aux);
		    ui.warning("--- The auxilliary archive of reference is now opened");
		}
		else
		    opt.set_auxilliary_ref(NULL);
	    }

		// now we can merge the archive

	    ui.warning("--- Proceeding to the merging operation...");
	    libdar::archive target = libdar::archive(ui,
						     archpath,
						     ref,
						     basename,
						     extension,
						     opt,
						     progressive_report);

		// as the object being local to the local block
		// it will be destroyed automatically (and the archive
		// will be closed) once we will have exit this local block
	    ui.warning("--- Merging operation completed");
	}
	catch(...)
	{
	    if(ref != NULL)
		delete ref;
	    if(aux != NULL)
		delete aux;
	    throw;
	}
	if(ref != NULL)
	    delete ref;
	if(aux != NULL)
	    delete aux;
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }
}

