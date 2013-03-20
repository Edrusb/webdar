    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "exceptions.hpp"
    //
#include "saisie.hpp"

using namespace std;

const string saisie::closing = "saisie_closing";

saisie::saisie():
    sessions("/", "Other sessions"),
    archive_form("Update"),
    archive("Archive Basename",
	     html_form_input::text,
	    "",
	    50),
    close("Do you really want to close this session?", false)
{
    html_text text;

	// configuration of "choice"
    choice.add_entry("about", "Main Page");
    choice.add_entry("restore", "Restoration");
    choice.add_entry("compare", "Comparison");
    choice.add_entry("test", "Testing");
    choice.add_entry("list", "Listing");
    choice.add_entry("create", "Creation");
    choice.add_entry("isolate", "Isolation");
    choice.add_entry("merge", "Merging");
    choice.add_entry("filters", "Filters");
    choice.add_entry("sep", "");
    choice.add_entry("close", "Close Session");
    choice.css_float(css::fl_left);
    adopt(&choice);

	// Configuring show_archive
    archive_form.adopt(&archive);
    show_archive.adopt(&archive_blank);
    show_archive.adopt(&archive_form);
    adopt(&show_archive);

	// configuration of sessions
    sessions.css_float(css::fl_left);
    sessions.css_float_clear(css::fc_left);
    adopt(&sessions);

	// configuration of "select" sub-pages

	//  configuration of the about sub-page
    text.clear();
    text.add_text(1,"WEBDAR");
    text.add_paragraph();
    text.add_text(4,string("version ") + WEBDAR_VERSION);
    text.add_paragraph();
    text.add_text(0, "by Denis CORBIN");
    about.adopt_static_html(text.get_body_part());
    select.adopt(&about);

	// configuration of the restore sub-page
    text.clear();
    text.add_text(1, "restoration page");
    text.add_nl();
    text.add_text(0, "to be implemented");
    restore.adopt_static_html(text.get_body_part());
    select.adopt(&restore);

    compare.adopt_static_html(text.get_body_part());
    select.adopt(&compare);

    test.adopt_static_html(text.get_body_part());
    select.adopt(&test);


    select.adopt(&list);

    create.adopt_static_html(text.get_body_part());
    select.adopt(&create);

    isolate.adopt_static_html(text.get_body_part());
    select.adopt(&isolate);

    merge.adopt_static_html(text.get_body_part());
    select.adopt(&merge);

    filters.adopt_static_html(text.get_body_part());
    select.adopt(&filters);

	// configuration of the sep sub-page
    text.clear();
    text.add_text(1, "BUG: THIS separation PAGE SHOULD NEVER SHOW");
    sep.adopt_static_html(text.get_body_part());
    select.adopt(&sep);

    select.adopt(&close);

	/// configuration of "select"
    select.css_margin_left("9.4em");
    select.set_mode(0);
    adopt(&select);

	// define the closing event for this
    register_name(closing);

	// attaching the "changed" event of the menu "choice" to "this" saisie object
    choice.record_actor_on_event(this);
    on_event(""); // manually triggering the event for the initial setup
}

void saisie::on_event(const std::string & event_name)
{
    if(event_name != menu::changed && event_name != "")
	throw WEBDAR_BUG;

	// menu "choice" changed

    set_title(string("WEBDAR - ") + choice.get_current_label());
    switch(choice.get_current_mode())
    {
    case 0:
    case 8:
    case 9:
    case 10:
	show_archive.set_mode(0);
	break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
	show_archive.set_mode(1);
	break;
    default:
	throw WEBDAR_BUG;
    }
    select.set_mode(choice.get_current_mode());
}

string saisie::get_body_part(const chemin & path,
			     const request & req)
{
    chemin sub_path = path;
    string ret;

    if(!sub_path.empty())
	sub_path.pop_front();

	// now we can generate in return the whole HTML code for "this" object
    ret = html_page::get_body_part(path, req);
    if(choice.get_current_label() == "close")
    {
	if(close.get_value())
	{
	    act(closing);
	    set_title("Session closed");
	    set_refresh_redirection(0, "/");
	    ret = html_page::get_body_part(path, req);
	}
    }

    return ret;
}

