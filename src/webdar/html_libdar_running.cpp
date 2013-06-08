    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "webdar_css_style.hpp"
    //
#include "html_libdar_running.hpp"

using namespace std;

const string html_libdar_running::ask_end_libdar = "html_libdar_running_ask_end_libdar";
const string html_libdar_running::force_end_libdar = "html_libdar_running_force_end_libdar";
const string html_libdar_running::close_libdar_screen = "html_libdar_running_close_libdar_screen";

html_libdar_running::html_libdar_running():
    html_page("Libdar thread current status"),
    ask_close("Stop libdar thread", ask_end_libdar),
    force_close("Kill libdar thread", force_end_libdar),
    finish("Close", close_libdar_screen)
{
    global.css_margin("1em");
    global.css_padding("1em");
    global.css_border_style(css::bd_all, css::bd_inset);
    global.css_border_width(css::bd_all, css::bd_medium);

    web_ui.css_width("90%", true);
    stats.css_width("90%", true);

    webdar_style_normal_button(ask_close);
    ask_close.css_float(css::fl_right);

    webdar_style_normal_button(force_close);
    force_close.css_float(css::fl_right);

    webdar_style_normal_button(finish);
    finish.css_float(css::fl_right);

    global.adopt(&web_ui);
    global.adopt(&stats);
    global.adopt(&ask_close);
    global.adopt(&force_close);
    global.adopt(&finish);
    adopt(&global);

    register_name(ask_end_libdar);
    register_name(force_end_libdar);
    register_name(close_libdar_screen);

    set_mode(normal);

    ask_close.record_actor_on_event(this, ask_end_libdar);
    force_close.record_actor_on_event(this, force_end_libdar);
    finish.record_actor_on_event(this, close_libdar_screen);
}

string html_libdar_running::get_body_part(const chemin & path,
					  const request & req)
{
    if(web_ui.can_refresh() && mode != finished)
	set_refresh_redirection(1, req.get_uri().get_path().display(false));
    else
	set_refresh_redirection(0, ""); // disable refresh

    return html_page::get_body_part(path, req);
}


void html_libdar_running::clear()
{
    set_mode(normal);
    web_ui.clear();
    stats.clear_counters();
    stats.clear_labels();
}


void html_libdar_running::set_mode(mode_type m)
{
    switch(m)
    {
    case normal:
	ask_close.set_visible(true);
	force_close.set_visible(false);
	finish.set_visible(false);
	break;
    case end_asked:
	ask_close.set_visible(false);
	force_close.set_visible(true);
	finish.set_visible(false);
	break;
    case end_forced:
	ask_close.set_visible(false);
	force_close.set_visible(false);
	finish.set_visible(false);
	break;
    case finished:
	ask_close.set_visible(false);
	force_close.set_visible(false);
	finish.set_visible(true);
	break;
    default:
	throw WEBDAR_BUG;
    }

    mode = m;
}
