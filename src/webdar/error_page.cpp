    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_fieldset.hpp"
#include "html_form_radio.hpp"
#include "html_div.hpp"

    //
#include "error_page.hpp"

using namespace std;

error_page::error_page(unsigned int status_code,
		       const std::string & reason)
{
	// tests:
    html_form *form = NULL;
    html_form_input *input = NULL;
    html_form_fieldset *fs = NULL;
    html_form_radio *radio = NULL;
    html_div *div = NULL;

    status = status_code;
    msg = reason;
    page = NULL;
    body = NULL;
    text = NULL;

    try
    {
	page = new (nothrow) html_page(msg);
	body = new (nothrow) html_text();
	text = new (nothrow) html_text();
	form = new (nothrow) html_form("Update");
	input = new (nothrow) html_form_input("this is the label",
					      html_form_input::text,
					      "hello",
					      25);
	fs = new (nothrow) html_form_fieldset("Tests de formulaire");
	radio = new (nothrow) html_form_radio();
	div = new (nothrow) html_div();

	if(page == NULL
	   || body == NULL
	   || text == NULL
	   || form == NULL
	   || radio == NULL
	   || fs == NULL
	   || div == NULL
	   || input == NULL)
	    throw exception_memory();

	page->css_color("Blue");
	page->css_background_color("Orange");
	page->css_padding("1em");

	body->css_font_weight_bold();
	body->css_color("White");
	body->css_background_color("Red");
	body->css_text_align(css::al_center);
	body->css_padding("2em");
	body->css_float(css::fl_left);
	body->css_border_width(css::bd_all, css::bd_thick);
	body->css_border_style(css::bd_all, css::bd_dashed);
	body->css_border_color(css::bd_all, "rgb(100,0,0)");
	body->css_width("6em", false);

	div->css_border_color(css::bd_all, "Blue");
	div->css_border_style(css::bd_all, css::bd_double);
	div->css_border_width(css::bd_all, css::bd_thin);
	div->css_margin_left("13em");

	text->css_margin_left("2em");
	text->css_background_color("White");
	text->css_border_style(css::bd_bottom, css::bd_outset);
	text->css_border_style(css::bd_right, css::bd_outset);
	text->css_border_width(css::bd_bottom, css::bd_medium);
	text->css_border_width(css::bd_right, css::bd_medium);
	text->css_border_color(css::bd_bottom, "Yellow");
	text->css_border_color(css::bd_right, "Yellow");

	input->css_margin("1em");
	input->css_background_color("Grey");

	radio->css_background_color("Yellow");
	radio->add_choice("ete", "summer time");
	radio->add_choice("automne", "Autumn time");
	radio->add_choice("hiver", "Winter time");
	radio->add_choice("printemps", "Spring time");
	radio->set_selected(3);

	form->css_text_align(css::al_right);
    }
    catch(...)
    {
	if(page != NULL)
	    delete page;
	if(body != NULL)
	    delete body;
	if(form != NULL)
	    delete form;
	if(fs != NULL)
	    delete fs;
	if(input != NULL)
	    delete input;
	if(radio != NULL)
	    delete radio;
	if(div != NULL)
	    delete div;
	throw;
    }

    fs->give(radio);
    form->give(input);
    form->give(fs);
    div->give(text);
    div->give(form);
    page->give(body);
    page->give(div);
	// now we only own the page object
	// all other pointed objects are owned by 'page' or its descendants
	// in the body builder tree. Destroying 'page' will destroy them
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
