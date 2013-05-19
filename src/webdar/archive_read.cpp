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
    arch_path("Archive path",
	      html_form_input::text,
	      "",
	      50),
    archive("Archive basename",
	    html_form_input::text,
	    "",
	    30),
    show_read_options("Show archive reading options",
		      html_form_input::check,
		      "",
		      1)
{

	/// web components layout
    form.adopt(&arch_path);
    form.adopt(&archive);
    form.adopt(&show_read_options);
    fs.adopt(&form);
    fs.adopt(&opt_read);
    adopt(&fs);

	/// events
    show_read_options.record_actor_on_event(this, html_form_input::changed);
    on_event(html_form_input::changed); // set the object in a coherent status
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
