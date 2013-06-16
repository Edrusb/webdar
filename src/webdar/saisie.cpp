    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "exceptions.hpp"
#include "webdar_css_style.hpp"
    //
#include "saisie.hpp"

using namespace std;

const string saisie::event_closing = "saisie_closing";
const string saisie::event_restore = "saisie_restore";
const string saisie::event_compare = "saisie_compare";
const string saisie::event_test    = "saisie_test";
const string saisie::event_list    = "saisie_list";
const string saisie::event_create  = "saisie_create";
const string saisie::event_isolate = "saisie_isolate";
const string saisie::event_merge   = "saisie_merge";

saisie::saisie():
    show_archive_form_options("Update"),
    show_archive_fs_options("Options details"),
    archread("Source archive"),
    show_operation_options("Show operation options",
			   html_form_input::check,
			   "",
			   1),
    close("Do you really want to close this session?", false),
    go_extract("Restore", event_restore),
    go_compare("Compare", event_compare),
    go_test("Test", event_test),
    go_list("List", event_list),
    go_create("Create", event_create),
    go_isolate("Isolate", event_isolate),
    go_merge("Merge", event_merge),
    fs_root_fs("Restoration parameters"),
    fs_root("Directory to take as root for restoration", html_form_input::text, "", 30),
    fs_root_form("Update")
{
    html_text text;

    status = st_idle;

    webdar_style_normal_button(go_extract);
    webdar_style_normal_button(go_compare);
    webdar_style_normal_button(go_test);
    webdar_style_normal_button(go_list);
    webdar_style_normal_button(go_create);
    webdar_style_normal_button(go_isolate);
    webdar_style_normal_button(go_merge);

    go_extract.css_float(fl_right);
    go_compare.css_float(fl_right);
    go_test.css_float(fl_right);
    go_list.css_float(fl_right);
    go_create.css_float(fl_right);
    go_isolate.css_float(fl_right);
    go_merge.css_float(fl_right);

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

	// Configuring archive_show
    archive_show.adopt(&archread);
    show_archive_form_options.adopt(&show_operation_options);
    show_archive_fs_options.adopt(&show_archive_form_options);
    archive_show.adopt(&show_archive_fs_options);
    right_pan.adopt(&archive_show);

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
    div_about.adopt_static_html(text.get_body_part());
    select.adopt(&div_about);

	// separator
    select.adopt(&div_sep0);

	// configuration of the restore sub-page
    div_extract.adopt(&fs_root_fs);
    fs_root_fs.adopt(&fs_root_form);
    fs_root_form.adopt(&fs_root);
    div_extract.adopt(&extract);
    div_extract.adopt(&go_extract);
    select.adopt(&div_extract);

	// comparison sub-page
    div_compare.adopt(&compare);
    div_compare.adopt(&go_compare);
    select.adopt(&div_compare);

	// testing sub-page
    div_test.adopt(&test);
    div_test.adopt(&go_test);
    select.adopt(&div_test);

	// listing sub-page
    text.clear();
    text.add_text(1,"NOT YET IMPLEMENTED");
    div_list.adopt_static_html(text.get_body_part());
    div_list.adopt(&list);
    div_list.adopt(&go_list);
    select.adopt(&div_list);

	// creation sub-page
    div_create.adopt(&create);
    div_create.adopt(&go_create);
    select.adopt(&div_create);

	// isolation sub-page
    div_isolate.adopt_static_html(text.get_body_part());
    div_isolate.adopt(&go_isolate);
    select.adopt(&div_isolate);

	// merging sub-page
    div_merge.adopt_static_html(text.get_body_part());
    div_merge.adopt(&go_merge);
    select.adopt(&div_merge);

	// separator
    select.adopt(&div_sep1);

	// filter sub-page
    div_filters.adopt_static_html(text.get_body_part());
    select.adopt(&div_filters);

	// repository sub-page
    div_repo.adopt_static_html(text.get_body_part());
    select.adopt(&div_repo);

	// other sessions sub_page
    select.adopt(&div_sess);

	// separator
    select.adopt(&div_sep2);

    text.clear();
    text.add_text(1, "BUG: THIS separation PAGE SHOULD NEVER SHOW");
    div_sep0.adopt_static_html(text.get_body_part());
    div_sep1.adopt_static_html(text.get_body_part());
    div_sep2.adopt_static_html(text.get_body_part());

	// close session sub_page
    select.adopt(&close);

	/// configuration of "select"
    select.css_margin_left("9.4em");
    select.set_mode(0);
    right_pan.adopt(&select);
    adopt(&right_pan);

	// define the closing event for this
    register_name(event_closing);

	// attaching the "changed" event of the menu "choice" to "this" saisie object
    choice.record_actor_on_event(this);
	// attaching the "changed" event of the show_operation_options checkbox to "this"
    show_operation_options.record_actor_on_event(this, html_form_input::changed);
    on_event(""); // manually triggering the event for the initial setup

    go_extract.record_actor_on_event(this, event_restore);
    go_compare.record_actor_on_event(this, event_compare);
    go_test.record_actor_on_event(this, event_test);
    go_list.record_actor_on_event(this, event_list);
    go_create.record_actor_on_event(this, event_create);
    go_isolate.record_actor_on_event(this, event_isolate);
    go_merge.record_actor_on_event(this, event_merge);
    register_name(event_restore);
    register_name(event_compare);
    register_name(event_test);
    register_name(event_list);
    register_name(event_create);
    register_name(event_isolate);
    register_name(event_merge);
}

string saisie::get_body_part(const chemin & path,
			     const request & req)
{
    chemin sub_path = path;
    string ret;

    if(!sub_path.empty())
	sub_path.pop_front();

    status = st_idle;

	// now we can generate in return the whole HTML code for "this" object
    set_refresh_redirection(0,""); // clearing redirection that could have been set previously
    ret = html_page::get_body_part(path, req);
    if(choice.get_current_label() == "close")
    {
	if(close.get_value())
	{
	    act(event_closing);
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

void saisie::on_event(const std::string & event_name)
{
    if(event_name == html_menu::changed
       || event_name == html_form_input::changed
       || event_name == "")
    {
	// menu "choice" changed

	set_title(string("Webdar - ") + choice.get_current_label());
	if(choice.get_current_label() == "restore"
	   || choice.get_current_label() == "compare"
	   || choice.get_current_label() == "test"
	   || choice.get_current_label() == "list"
	   || choice.get_current_label() == "isolate"
	   || choice.get_current_label() == "merge")
	    archive_show.set_visible(true);
	else
	    archive_show.set_visible(false);
	select.set_mode(choice.get_current_mode());

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
    else if(event_name == event_restore
	    || event_name == event_compare
	    || event_name == event_test
	    || event_name == event_list
	    || event_name == event_create
	    || event_name == event_isolate
	    || event_name == event_merge)
    {
	if(event_name == event_restore)
	    status = st_restore;
	else if(event_name == event_compare)
	    status = st_compare;
	else if(event_name == event_test)
	    status = st_test;
	else if(event_name == event_list)
	    status = st_list;
	else if(event_name == event_create)
	    status = st_create;
	else if(event_name == event_isolate)
	    status = st_isolate;
	else if(event_name == event_merge)
	    status = st_merge;
	else
	    throw WEBDAR_BUG;
	act(event_name); // propagate the event to the subscribers
    }
    else
	throw WEBDAR_BUG;
}

const string & saisie::get_archive_path() const
{
    switch(status)
    {
    case st_idle:
	throw WEBDAR_BUG;
    case st_restore:
    case st_compare:
    case st_test:
    case st_list:
	return archread.get_archive_path();
	break;
    case st_create:
	return create.get_archive_path();
	break;
    case st_isolate:
	throw exception_feature("isolation operation");
    case st_merge:
	throw exception_feature("merging operation");
    default:
	throw WEBDAR_BUG;
    }
}

const string & saisie::get_archive_basename() const
{
    switch(status)
    {
    case st_idle:
	throw WEBDAR_BUG;
    case st_restore:
    case st_compare:
    case st_test:
    case st_list:
	return archread.get_archive_basename();
	break;
    case st_create:
	return create.get_archive_basename();
	break;
    case st_isolate:
	throw exception_feature("isolation operation");
    case st_merge:
	throw exception_feature("merging operation");
    default:
	throw WEBDAR_BUG;
    }
}

const libdar::archive_options_read & saisie::get_read_options() const
{
    if(status != st_restore
       && status != st_compare
       && status != st_test
       && status != st_list)
	throw WEBDAR_BUG;

    return archread.get_read_options();
}

const libdar::archive_options_extract saisie::get_extraction_options() const
{
    if(status != st_restore)
	throw WEBDAR_BUG;

    return extract.get_options();
}

const libdar::archive_options_diff saisie::get_comparison_options() const
{
    if(status != st_compare)
	throw WEBDAR_BUG;

    return compare.get_options();
}

const libdar::archive_options_test saisie::get_testing_options() const
{
    if(status != st_test)
	throw WEBDAR_BUG;

    return test.get_options();
}

const libdar::archive_options_listing saisie::get_listing_options() const
{
    if(status != st_list)
	throw WEBDAR_BUG;

    return list.get_options();
}

const html_options_create & saisie::get_creating_options() const
{
    if(status != st_create)
	throw WEBDAR_BUG;

    return create.get_options_create();
}


