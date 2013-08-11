    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_archive_merge.hpp"

using namespace std;

html_archive_merge::html_archive_merge():
    form("Update"),
    fs("Resulting archive"),
    sauv_path("Where to create the archive", html_form_input::text, "", 20),
    basename("Archive basename", html_form_input::text, "", 10),
    show_options("Show merging options", html_form_input::check, "", 1),
    reference("Archive of reference")
{
    fs.adopt(&sauv_path);
    fs.adopt(&basename);
    fs.adopt(&show_options);
    form.adopt(&fs);
    adopt(&form);
    adopt(&reference);
    adopt(&options);

    show_options.record_actor_on_event(this, html_form_input::changed);

    on_event("");
}

html_archive_merge::html_archive_merge(const html_archive_merge & ref):
    form(""),
    fs(""),
    sauv_path("", html_form_input::text, "", 20),
    basename("", html_form_input::text, "", 10),
    show_options("", html_form_input::check, "", 1),
    reference("")
{
    throw WEBDAR_BUG;
}

string html_archive_merge::get_body_part(const chemin & path,
				     const request & req)
{
    return get_body_part_from_all_children(path, req);
}


void html_archive_merge::on_event(const std::string & event_name)
{
    options.set_visible(show_options.get_value_as_bool());
}
