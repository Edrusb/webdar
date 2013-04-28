    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "archive_read.hpp"

using namespace std;

archive_read::archive_read(const string & archive_description):
    form("Update"),
    fs(archive_description),
    archive("Archive Basename",
	    html_form_input::text,
	    "",
	    30),
    show_read_options("Show archive reading options",
		      html_form_input::check,
		      "",
		      1)
{
    ptr = NULL;

	/// web components layout
    form.adopt(&fs);
    fs.adopt(&archive);
    fs.adopt(&show_read_options);
    fs.adopt(&opt_read);
    adopt(&form);

	/// events
    show_read_options.record_actor_on_event(this, html_form_input::changed);
    on_event(html_form_input::changed); // set the object in a coherent status
}

archive_read::archive_read(const archive_read & ref):
    form(""),
    fs(""),
    archive("",
	    html_form_input::text,
	    "",
	    30),
    show_read_options("",
		      html_form_input::check,
		      "",
		      1)
{
    throw WEBDAR_BUG;
}

void archive_read::open_archive(/* web_user_interaction required here */)
{
    if(ptr != NULL)
	throw WEBDAR_BUG;

    try
    {
/*

        A IMPLEMENTER

	    /// split archive.get_value() in path and basename
	libdar::path chem = ...;
	string::basename;
	ptr = new archive(web_interaction,
			  chem,
			  basename,
			  /// extension
			  opt_read.get_options());

	if(ptr == NULL)
	    throw ...
*/
    }
    catch(libdar::Egeneric & e)
    {
/*

  A IMPLEMENTER

*/
    }

}

libdar::archive & archive_read::get_archive()
{
    if(ptr == NULL)
	throw WEBDAR_BUG;
    return *ptr;
}

void archive_read::close_archive()
{
    if(ptr == NULL)
	throw WEBDAR_BUG;
    delete ptr;
    ptr = NULL;
}

string archive_read::get_body_part(const chemin & path,
				   const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void archive_read::on_event(const std::string & event_name)
{
    if(event_name != html_form_input::changed)
	throw WEBDAR_BUG;

    opt_read.set_visible(show_read_options.get_value_as_bool());
}
