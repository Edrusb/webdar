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
    go_merge("Merge", event_merge)
{
    html_text text;
    css tmp_set;
    css tmp_set_url;

	// Common aspects
    box_off.css_border_style(css::bd_all, css::bd_solid, true);
    box_off.css_border_width(css::bd_all, css::bd_medium, true);
    box_off.css_width("8em", true, true);
    box_off.css_padding("0.5em", true);
    box_off.css_margin("0.2em", true);
    box_off.css_text_align(al_center, true);

	// copy common aspects to box_off and box_void
    box_on.css_inherit_from(box_off);
    box_void.css_inherit_from(box_off);
    box_void.css_border_style(css::bd_all, css::bd_none);

	// box_off and tmp_norm COLORS
    tmp_set.css_clear_attributes();
    tmp_set.css_color(COLOR_MENU_FRONT_OFF, true);
    tmp_set.css_background_color(COLOR_MENU_BACK_OFF, true);
    tmp_set.css_font_weight_bold(true);
    tmp_set.css_font_style_italic(true);
    tmp_set.css_text_decoration(css::dc_none, true);
    box_off.css_inherit_from(tmp_set);
    url_normal.set_style_link(tmp_set);
    url_normal.set_style_visited(tmp_set);
    box_off.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF, true);

	// Link Hover and Active in box_off
    tmp_set.css_color(COLOR_MENU_FRONT_HOVER_OFF, true);
    tmp_set.css_text_decoration(dc_underline, true);
    url_normal.set_style_hover(tmp_set);
    tmp_set.css_color(COLOR_MENU_FRONT_ACTIVE_OFF, true);
    url_normal.set_style_active(tmp_set);

	// box_on and tmp_select COLORS
    tmp_set.css_color(COLOR_MENU_FRONT_ON, true);
    tmp_set.css_background_color(COLOR_MENU_BACK_ON, true);
    tmp_set.css_font_weight_bold(true);
    tmp_set.css_font_style_normal(true);
    tmp_set.css_text_decoration(css::dc_none, true);
    box_on.css_inherit_from(tmp_set);
    url_selected.set_style_link(tmp_set);
    url_selected.set_style_visited(tmp_set);
    box_on.css_border_color(css::bd_all, COLOR_MENU_BORDER_ON, true);

	// Link Hover and Active in box_on
    tmp_set.css_color(COLOR_MENU_FRONT_HOVER_ON, true);
    tmp_set.css_text_decoration(dc_underline, true);
    url_selected.set_style_hover(tmp_set);
    tmp_set.css_color(COLOR_MENU_FRONT_ACTIVE_ON, true);
    url_selected.set_style_active(tmp_set);

    go_extract.css_inherit_from(box_off);
    go_compare.css_inherit_from(box_off);
    go_test.css_inherit_from(box_off);
    go_list.css_inherit_from(box_off);
    go_create.css_inherit_from(box_off);
    go_isolate.css_inherit_from(box_off);
    go_merge.css_inherit_from(box_off);

    go_extract.css_float(fl_right);
    go_compare.css_float(fl_right);
    go_test.css_float(fl_right);
    go_list.css_float(fl_right);
    go_create.css_float(fl_right);
    go_isolate.css_float(fl_right);
    go_merge.css_float(fl_right);

    go_extract.set_url_class(url_normal);
    go_compare.set_url_class(url_normal);
    go_test.set_url_class(url_normal);
    go_list.set_url_class(url_normal);
    go_create.set_url_class(url_normal);
    go_isolate.set_url_class(url_normal);
    go_merge.set_url_class(url_normal);

    css tmp_set2;
    tmp_set2.css_border_style(css::bd_all, css::bd_none);
    go_extract.set_url_css(tmp_set2);
    go_compare.set_url_css(tmp_set2);
    go_test.set_url_css(tmp_set2);
    go_list.set_url_css(tmp_set2);
    go_create.set_url_css(tmp_set2);
    go_isolate.set_url_css(tmp_set2);
    go_merge.set_url_css(tmp_set2);

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
    archive_show.adopt(&archread);
    show_archive_form_options.adopt(&show_operation_options);
    show_archive_fs_options.adopt(&show_archive_form_options);
    archive_show.adopt(&show_archive_fs_options);
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
    div_about.adopt_static_html(text.get_body_part());
    select.adopt(&div_about);

	// separator
    select.adopt(&div_sep0);

	// configuration of the restore sub-page
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

void saisie::on_event(const std::string & event_name)
{
    if(event_name == menu::changed
       || event_name == html_form_input::changed
       || event_name == "")
    {
	// menu "choice" changed

	set_title(string("WEBDAR - ") + choice.get_current_label());
	if(choice.get_current_label() == "restore"
	   || choice.get_current_label() == "compare"
	   || choice.get_current_label() == "test"
	   || choice.get_current_label() == "list"
	   || choice.get_current_label() == "isolate"
	   || choice.get_current_label() == "merge")
	    show_archive.set_mode(1);
	else
	    show_archive.set_mode(0);
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
    else
	if(event_name == event_restore
	   || event_name == event_compare
	   || event_name == event_test
	   || event_name == event_list
	   || event_name == event_create
	   || event_name == event_isolate
	   || event_name == event_merge)
	{
	    act(event_name); // propagate the event to the subscribers
	}
	else
	    throw WEBDAR_BUG;
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

