    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "webdar_css_style.hpp"
#include "tokens.hpp"

    //
#include "html_listing_page.hpp"

using namespace std;

const string html_listing_page::event_close = "html_listing_page::close";

html_listing_page::html_listing_page():
    tree(""),
    close("Close", event_close)
{
    css tmp;

    set_session_name("");

	// set css properties
    tree.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF);
    tree.css_border_width(css::bd_all, css::bd_medium);
    tree.css_border_style(css::bd_all, css::bd_double);
    tree.css_float(css::fl_left);
    tree.css_width("30%", false);

    webdar_style_normal_button(close);
    close.css_float(css::fl_right);
    close.css_float_clear(css::fc_both);

    tmp.css_clear_attributes();
    tmp.css_color(COLOR_TEXT);
    tmp.css_text_align(css::al_center);
    tmp.css_text_decoration(css::dc_none);
    bt_class.set_style_link(tmp);
    bt_class.set_style_visited(tmp);
    tmp.css_text_decoration(css::dc_underline);
    bt_class.set_style_hover(tmp);

	// the body_builder tree

    adopt(&bt_class);
    adopt(&tree);
    adopt(&close);

	//
    tree.set_css_classid(bt_class.get_class_id());

	// event binding
    register_name(event_close);
    close.record_actor_on_event(this, event_close);
}

void html_listing_page::on_event(const std::string & event_name)
{
    if(event_name != event_close)
	throw WEBDAR_BUG;
    act(event_close); // propagate the event
}

void html_listing_page::set_session_name(const string & session_name)
{
    string title = "Webdar - ";

    if(session_name != "")
	title += session_name + " - ";

    title += "Archive listing";
    set_title(title);
}
