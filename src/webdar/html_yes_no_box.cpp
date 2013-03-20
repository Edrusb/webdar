    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_fieldset.hpp"

    //
#include "html_yes_no_box.hpp"


using namespace std;

html_yes_no_box::html_yes_no_box(const std::string & message, bool default_value)
{
    html_form *tmp_form = new (nothrow) html_form("Confirm");
    html_form_fieldset *tmp_fieldset = new (nothrow) html_form_fieldset(message);
    html_form_radio *tmp_radio = new (nothrow) html_form_radio ();

    rd = NULL;
    form = NULL;
    try
    {
	if(tmp_form == NULL
	   || tmp_fieldset == NULL
	   || tmp_radio == NULL)
	    throw exception_memory();

	tmp_radio->add_choice("no", "No");
	tmp_radio->add_choice("yes", "Yes");
	tmp_radio->set_selected(default_value ? 1 : 0);

	tmp_fieldset->give(tmp_radio);
	rd = tmp_radio;
	tmp_radio = NULL;

	tmp_form->give(tmp_fieldset);
	tmp_fieldset = NULL;

	give(tmp_form);
	form = tmp_form;
	tmp_form = NULL;
    }
    catch(...)
    {
	if(tmp_form != NULL)
	    delete tmp_form;
	if(tmp_fieldset != NULL)
	    delete tmp_fieldset;
	if(tmp_radio != NULL)
	    delete tmp_radio;
	rd = NULL;
	throw;
    }
}
