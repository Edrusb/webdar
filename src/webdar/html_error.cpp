    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "webdar_css_style.hpp"

    //
#include "html_error.hpp"

using namespace std;

const string html_error::acknowledged = "html_error_acknowledged";
const string html_error::close_event = "html_error_button_close";
const string fixed_title1 = "Webdar - ";
const string fixed_title2 = "Libdar message";

html_error::html_error():
    html_page(fixed_title1+fixed_title2),
    close("Close", close_event)
{
    the_error.css_color("#FF0000");
    the_error.css_text_align(css::al_center);
    the_error.css_font_weight_bold();

    global.css_padding("1em");
    global.css_margin("1em");
    global.css_border_width(css::bd_all, css::bd_medium);
    global.css_border_style(css::bd_all, css::bd_inset);

    webdar_style_normal_button(close);
    close.css_float(fl_right);
    close.css_margin_right("1em");

    global.adopt(&the_error);
    adopt(&global);
    adopt(&close);

    close.record_actor_on_event(this, close_event);

    register_name(acknowledged);
}

void html_error::set_message(const std::string & msg)
{
    the_error.clear();
    the_error.add_text(3, msg);
}

void html_error::on_event(const std::string & event_name)
{
    act(acknowledged);
}

void html_error::set_session_name(const std::string & sessname)
{
    set_title(fixed_title1 + sessname + " - " + fixed_title2);
}
