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
    archive_form("Update"),
    archive("Archive Basename",
	     html_form_input::text,
	    "",
	    30),
    show_read_options("Show archive options",
		      html_form_input::check,
		      "",
		      1),
    show_operation_options("Show operation options",
			   html_form_input::check,
			   "",
			   1),
    close("Do you really want to close this session?", false)
{
    html_text text;

	// right pan configuration
    right_pan.css_float(fl_left, true);

	// configuration of "choice"
    choice.add_entry("about", "Main Page");
    choice.add_entry("sep0", "");
    choice.add_entry("restore", "Restoration");
    choice.add_entry("compare", "Comparison");
    choice.add_entry("test", "Testing");
    choice.add_entry("list", "Listing");
    choice.add_entry("create", "Creation");
    choice.add_entry("isolate", "Isolation");
    choice.add_entry("merge", "Merging");
    choice.add_entry("sep1", "");
    choice.add_entry("filters", "Filters");
    choice.add_entry("repo", "Repositories");
    choice.add_entry("sess", "Other Sessions");
    choice.add_entry("sep2", "");
    choice.add_entry("close", "Close Session");
    choice.css_float(css::fl_left);
    adopt(&choice);

	// Configuring show_archive
    archive_form.adopt(&archive);
    archive_form.adopt(&show_read_options);
    archive_form.adopt(&show_operation_options);
    archive_show.adopt(&archive_form);
    archive_show.adopt(&opt_read);
    show_archive.adopt(&archive_blank);
    show_archive.adopt(&archive_show);
    right_pan.adopt(&show_archive);

	// configuration of the sub-pages brought by "select"

	//  configuration of the about sub-page
    text.clear();
    text.css_text_align(css::al_center, true);
    text.css_margin("2em", true);
    text.add_text(1,"WEBDAR");
    text.add_paragraph();
    text.add_text(4,string("version ") + WEBDAR_VERSION);
    text.add_paragraph();
    text.add_text(0, "by Denis CORBIN");
    about.adopt_static_html(text.get_body_part());
    select.adopt(&about);

	// separator
    select.adopt(&sep0);

	// configuration of the restore sub-page
    select.adopt(&extract);

	// comparison sub-page
    select.adopt(&compare);

	// testing sub-page
    select.adopt(&test);

	// listing sub-page
    text.clear();
    text.add_text(1,"NOT YET IMPLEMENTED");
    select.adopt(&list);

	// creation sub-page
    create.adopt_static_html(text.get_body_part());
    select.adopt(&create);

	// isolation sub-page
    isolate.adopt_static_html(text.get_body_part());
    select.adopt(&isolate);

	// merging sub-page
    merge.adopt_static_html(text.get_body_part());
    select.adopt(&merge);

	// separator
    select.adopt(&sep1);

	// filter sub-page
    filters.adopt_static_html(text.get_body_part());
    select.adopt(&filters);

	// repository sub-page
    repo.adopt_static_html(text.get_body_part());
    select.adopt(&repo);

	// other sessions sub_page
    select.adopt(&sess);

	// separator
    select.adopt(&sep2);

    text.clear();
    text.add_text(1, "BUG: THIS separation PAGE SHOULD NEVER SHOW");
    sep0.adopt_static_html(text.get_body_part());
    sep1.adopt_static_html(text.get_body_part());
    sep2.adopt_static_html(text.get_body_part());

	// close session sub_page
    select.adopt(&close);

	/// configuration of "select"
    select.css_margin_left("9.4em");
    select.set_mode(0);
    right_pan.adopt(&select);
    adopt(&right_pan);

	// define the closing event for this
    register_name(closing);

	// attaching the "changed" event of the menu "choice" to "this" saisie object
    choice.record_actor_on_event(this);
    show_read_options.record_actor_on_event(this, html_form_input::changed);
    show_operation_options.record_actor_on_event(this, html_form_input::changed);
    on_event(""); // manually triggering the event for the initial setup
}

void saisie::on_event(const std::string & event_name)
{
    if(event_name != menu::changed
       && event_name != html_form_input::changed
       && event_name != "")
	throw WEBDAR_BUG;

	// menu "choice" changed

    set_title(string("WEBDAR - ") + choice.get_current_label());
    if(choice.get_current_label() == "restore"
       || choice.get_current_label() == "compare"
       || choice.get_current_label() == "test"
       || choice.get_current_label() == "list"
       || choice.get_current_label() == "create"
       || choice.get_current_label() == "isolate"
       || choice.get_current_label() == "merge")
	show_archive.set_mode(1);
    else
	show_archive.set_mode(0);
    select.set_mode(choice.get_current_mode());

    if(show_read_options.get_value_as_bool())
	opt_read.set_visible(true);
    else
	opt_read.set_visible(false);
    if(show_operation_options.get_value_as_bool())
    {
	extract.set_visible(true);
	compare.set_visible(true);
	test.set_visible(true);
	list.set_visible(true);
    }
    else
    {
	extract.set_visible(false);
	compare.set_visible(false);
	test.set_visible(false);
	list.set_visible(false);
    }
}

string saisie::get_body_part(const chemin & path,
			     const request & req)
{
    chemin sub_path = path;
    string ret;

    if(!sub_path.empty())
	sub_path.pop_front();

	// now we can generate in return the whole HTML code for "this" object
    set_refresh_redirection(0,""); // clearing redirection that could have been set previously
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
    else
	if(choice.get_current_label() == "sess")
	{
	    set_title("Redirection to all user sessions");
	    set_refresh_redirection(0, "/");
	    ret = html_page::get_body_part(path, req);
	    choice.set_current_mode(choice.get_previous_mode());
	}

    return ret;
}

