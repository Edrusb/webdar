    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_fieldset.hpp"
#include "html_form_radio.hpp"

    //
#include "error_page.hpp"

using namespace std;

error_page::error_page(unsigned int status_code,
		       const std::string & reason)
{
    html_table *table = NULL;
	// tests:
    html_form *form = NULL;
    html_form_input *input = NULL;
    html_form_fieldset *fs = NULL;
    html_form_radio *radio = NULL;

    status = status_code;
    msg = reason;
    page = NULL;
    body = NULL;
    text = NULL;

    try
    {
	page = new (nothrow) html_page(msg);
	table = new (nothrow) html_table(1);
	body = new (nothrow) html_text(1);
	text = new (nothrow) html_text(0);
	form = new (nothrow) html_form("Update");
	input = new (nothrow) html_form_input("this is the label",
					      html_form_input::text,
					      "hello",
					      10);
	fs = new (nothrow) html_form_fieldset("Tests de formulaire");
	radio = new (nothrow) html_form_radio();

	if(page == NULL
	   || table == NULL
	   || body == NULL
	   || text == NULL
	   || form == NULL
	   || radio == NULL
	   || fs == NULL
	   || input == NULL)
	    throw exception_memory();

	table->set_border(5);
	radio->add_choice("ete", "summer time");
	radio->add_choice("automne", "Autumn time");
	radio->add_choice("hiver", "Winter time");
	radio->add_choice("printemps", "Spring time");
	radio->set_selected(3);
    }
    catch(...)
    {
	if(page != NULL)
	    delete page;
	if(table != NULL)
	    delete table;
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
	throw;
    }

    table->body_builder::give(body);
    table->body_builder::give(text);
    table->body_builder::give(form);
    form->give(input);
    form->give(fs);
    fs->give(radio);
    page->body_builder::give(table);
	// now we only own the page object
	// all other pointed objects are owned by 'page' or its descendants
	// in the body builder tree. Destroying 'page' will destroy them
}

void error_page::set_message_body(const std::string & message)
{
    if(body == NULL)
	throw WEBDAR_BUG;
    body->clear(1);
    body->add_text(true, false, false, message);
}

void error_page::set_return_uri(const uri &ref, const string & label)
{
    if(text == NULL)
	throw WEBDAR_BUG;
    text->clear(0);
    text->add_text(false, false, false, html_url(ref.get_string(), label).get_body_part());
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
