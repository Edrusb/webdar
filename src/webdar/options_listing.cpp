    // C system header files
extern "C"
{

}

    // C++ system header files
#include <new>

    // webdar headers
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_div.hpp"
#include "exceptions.hpp"

    //
#include "options_listing.hpp"

using namespace std;

options_listing::options_listing()
{
	// anonymous objects
    html_div *div = NULL;
    html_form *form = NULL;
    html_form_fieldset *fs = NULL;
    html_form_input *tmp_details = NULL;
    html_form_input *tmp_unsaved = NULL;
    html_form_input *tmp_ea = NULL;


	// object fields
    info_details = NULL;
    filter_unsaved = NULL;
    display_ea = NULL;

    try
    {
	div = new (nothrow) html_div();
	form = new (nothrow) html_form("Update");
	fs = new (nothrow) html_form_fieldset("Listing options");
	tmp_details = new (nothrow) html_form_input("Detailed informations",
						    html_form_input::check,
						    "",
						    0);
	tmp_unsaved = new (nothrow) html_form_input("Filter out unsaved files",
						    html_form_input::check,
						    "",
						    0);
	tmp_ea = new (nothrow) html_form_input("Display Extended Attributes",
						html_form_input::check,
						"",
						0);
	if(form == NULL
	   || fs == NULL
	   || tmp_details == NULL
	   || tmp_unsaved == NULL
	   || tmp_ea == NULL)
	    throw exception_memory();

	info_details = tmp_details;
	filter_unsaved = tmp_unsaved;
	display_ea = tmp_ea;

	fs->give(tmp_details);
	tmp_details = NULL;
	fs->give(tmp_unsaved);
	tmp_unsaved = NULL;
	fs->give(tmp_ea);
	tmp_ea = NULL;
	form->give(fs);
	fs = NULL;
	div->give(form);
	form = NULL;
	record_child(div);
	div = NULL;
    }
    catch(...)
    {
	if(tmp_details != NULL)
	    delete info_details;
	if(tmp_unsaved != NULL)
	    delete filter_unsaved;
	if(tmp_ea != NULL)
	    delete display_ea;
	if(fs != NULL)
	    delete fs;
	if(form != NULL)
	    delete form;
	if(div != NULL)
	    delete div;
	throw;
    }
}

options_listing::~options_listing()
{
    info_details = NULL;
    filter_unsaved = NULL;
    display_ea = NULL;
	// the body_builder parent destructor will destroy these objects
	// because they are "recorded" as child of "this".
}


string options_listing::get_body_part(const chemin & path,
				       const request & req)
{
    return get_body_part_from_all_children(path, req);
}


libdar::archive_options_listing options_listing::get_options()
{
    libdar::archive_options_listing ret;

    if(info_details == NULL
       || filter_unsaved == NULL
       || display_ea == NULL)
	throw WEBDAR_BUG;
    ret.set_info_details(info_details->get_value_as_bool());
    ret.set_filter_unsaved(filter_unsaved->get_value_as_bool());
    ret.set_display_ea(display_ea->get_value_as_bool());

    return ret;
}
