    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_archive_create.hpp"

using namespace std;

html_archive_create::html_archive_create():
    fs_root("Directory to take as root", html_form_input::text, "", 20),
    sauv_path("Directory to write archive into", html_form_input::text, "", 20),
    basename("Archive basename", html_form_input::text, "", 10),
    show_options("Show creation options", html_form_input::check, "", 1),
    form("Update")
{
    form.adopt(&fs_root);
    form.adopt(&sauv_path);
    form.adopt(&basename);
    form.adopt(&show_options);
    adopt(&form);
    adopt(&options);


    show_options.record_actor_on_event(this, html_form_input::changed);

    on_event("");
}

html_archive_create::html_archive_create(const html_archive_create & ref):
    fs_root("", html_form_input::text, "", 20),
    sauv_path("", html_form_input::text, "", 20),
    basename("", html_form_input::text, "", 10),
    show_options("", html_form_input::check, "", 1),
    form("")
{
    throw WEBDAR_BUG;
}

string html_archive_create::get_body_part(const chemin & path,
				     const request & req)
{
    return get_body_part_from_all_children(path, req);
}


void html_archive_create::on_event(const std::string & event_name)
{
    options.set_visible(show_options.get_value_as_bool());
}
