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
    fs_root("Directory to take as root for the backup", html_form_input::text, "", 20),
    sauv_path("Directory to write archive into", html_form_input::text, "", 20),
    basename("Archive basename", html_form_input::text, "", 10),
    show_options("Show creation options", html_form_input::check, "", 1),
    form("Update"),
    fs("Archive to create")
{
    fs.adopt(&fs_root);
    fs.adopt(&sauv_path);
    fs.adopt(&basename);
    fs.adopt(&show_options);
    form.adopt(&fs);
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
    form(""),
    fs("")
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
