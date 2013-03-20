    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "options_listing.hpp"
#include "html_div.hpp"
#include "tokens.hpp"
#include "menu.hpp"

    //
#include "error_page.hpp"

using namespace std;

error_page::error_page(unsigned int status_code,
		       const std::string & reason):
    page(reason)
{
	// objects's fields
    status = status_code;
    msg = reason;

	// setting css properties for each object

    page.css_color(COLOR_TEXT);
    page.css_background_color(COLOR_BACK);
    page.css_padding("1em");

    body.css_font_weight_bold();
    body.css_color("White");
    body.css_background_color("Red");
    body.css_text_align(css::al_center);
    body.css_padding("1em");
    body.css_float(css::fl_left);
    body.css_border_width(css::bd_all, css::bd_thick);
    body.css_border_style(css::bd_all, css::bd_dashed);
    body.css_border_color(css::bd_all, "rgb(100,0,0)");
    body.css_width("6em", false);

    div.css_margin_left("13em");

    text.css_float(css::fl_left);
    text.css_float_clear(css::fc_left);
    text.css_margin_left("2em");

	// building the body_builder tree

    div.adopt(&body);
    div.adopt(&text);
    page.adopt(&div);
}

void error_page::set_message_body(const std::string & message)
{
    body.clear();
    body.add_text(3, message);
}

void error_page::set_return_uri(const uri &ref, const string & label)
{
    text.clear();
    text.add_text(0, html_url(ref.get_string(), label).get_body_part());
}

answer error_page::give_answer(const request & req)
{
    answer ret;

    ret.set_status(status);
    ret.set_reason(msg);
    ret.add_body(page.get_body_part(req.get_uri().get_path(), req));

    return ret;
}

void error_page::prefix_has_changed()
{
    page.set_prefix(get_prefix());
}
