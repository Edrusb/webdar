    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_archive_isolate.hpp"

using namespace std;

html_archive_isolate::html_archive_isolate():
    fs("Isolated catalog to be create"),
    form("Update"),
    sauv_path("Where to create the archive", html_form_input::text, "", 20),
    basename("Archive basename", html_form_input::text, "", 10),
    ref("Source archive"),
    show_options("Show isolation options", html_form_input::check, "", 1)
{
    fs.adopt(&sauv_path);
    fs.adopt(&basename);
    fs.adopt(&show_options);
    form.adopt(&fs);
    adopt(&form);
    adopt(&ref);
    adopt(&options);

    show_options.record_actor_on_event(this, html_form_input::changed);
    on_event("");
}

html_archive_isolate::html_archive_isolate(const html_archive_isolate & ref):
    fs(""),
    form(""),
    sauv_path("", html_form_input::text, "", 20),
    basename("", html_form_input::text, "", 10),
    ref(""),
    show_options("", html_form_input::check, "", 1)
{
    throw WEBDAR_BUG;
}

string html_archive_isolate::get_body_part(const chemin & path,
				     const request & req)
{
    return get_body_part_from_all_children(path, req);
}


void html_archive_isolate::on_event(const std::string & event_name)
{
    options.set_visible(show_options.get_value_as_bool());
}
