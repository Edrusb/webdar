    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "options_listing.hpp"
#include "html_div.hpp"
#include "tokens.hpp"

    //
#include "error_page.hpp"

using namespace std;

error_page::error_page(unsigned int status_code,
		       const std::string & reason)
{
	// construction objects
    html_div *tmp_div = NULL;
    html_page *tmp_page = NULL;
    html_text *tmp_body = NULL;
    html_text *tmp_text = NULL;
	// FOR TESTING
    options_listing *tmp_opt = NULL;

	// objects's fields
    status = status_code;
    msg = reason;
    page = NULL;
    body = NULL;
    text = NULL;

    try
    {
	tmp_page = new (nothrow) html_page(msg);
	tmp_body = new (nothrow) html_text();
	tmp_text = new (nothrow) html_text();
	tmp_div = new (nothrow) html_div();
	tmp_opt = new (nothrow) options_listing();

	if(tmp_page == NULL
	   || tmp_body == NULL
	   || tmp_text == NULL
	   || tmp_opt == NULL
	   || tmp_div == NULL)
	    throw exception_memory();

	    // we record the address of the newly created objets
	    // only useful objects are record (tmp_div is not directly used outside the constructor)

	page = tmp_page;
	body = tmp_body;
	text = tmp_text;

	    // setting css properties for each object

	tmp_page->css_color(COLOR_TEXT);
	tmp_page->css_background_color(COLOR_BACK);
	tmp_page->css_padding("1em");

	tmp_body->css_font_weight_bold();
	tmp_body->css_color("White");
	tmp_body->css_background_color("Red");
	tmp_body->css_text_align(css::al_center);
	tmp_body->css_padding("1em");
	tmp_body->css_float(css::fl_left);
	tmp_body->css_border_width(css::bd_all, css::bd_thick);
	tmp_body->css_border_style(css::bd_all, css::bd_dashed);
	tmp_body->css_border_color(css::bd_all, "rgb(100,0,0)");
	tmp_body->css_width("6em", false);

	tmp_div->css_margin_left("13em");

	tmp_text->css_float(css::fl_left);
	tmp_text->css_float_clear(css::fc_left);
	tmp_text->css_margin_left("2em");

	    // building the body_builder tree

	tmp_div->give(tmp_opt);
	tmp_opt = NULL;

	tmp_page->give(tmp_body);
	tmp_body = NULL;
	tmp_page->give(tmp_text);
	tmp_text = NULL;
	tmp_page->give(tmp_div);
	tmp_div = NULL;
    }
    catch(...)
    {
	if(tmp_page != NULL)
	    delete tmp_page;
	if(tmp_body != NULL)
	    delete tmp_body;
	if(tmp_text != NULL)
	    delete tmp_text;
	if(tmp_div != NULL)
	    delete tmp_div;
	if(tmp_opt != NULL)
	    delete tmp_opt;
	throw;
    }
}

void error_page::set_message_body(const std::string & message)
{
    if(body == NULL)
	throw WEBDAR_BUG;
    body->clear();
    body->add_text(3, message);
}

void error_page::set_return_uri(const uri &ref, const string & label)
{
    if(text == NULL)
	throw WEBDAR_BUG;
    text->clear();
    text->add_text(0, html_url(ref.get_string(), label).get_body_part());
}

answer error_page::give_answer(const request & req)
{
    answer ret;

    ret.set_status(status);
    ret.set_reason(msg);
    ret.add_body(page->get_body_part(req.get_uri().get_path(), req));

    return ret;
}

void error_page::prefix_has_changed()
{
    if(page == NULL)
	throw WEBDAR_BUG;
    page->set_prefix(get_prefix());
}

void error_page::field_delete()
{
    if(page != NULL)
    {
	delete page;
	page = NULL;
    }

	// other objects are owned by 'page' directly or indirectly
	// they are deleted when page is deleted
    body = NULL;
    text = NULL;
}
