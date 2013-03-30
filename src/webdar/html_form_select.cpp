    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_form_select.hpp"

using namespace std;

const string html_form_select::changed = "html_form_select_changed";

string html_form_select::get_body_part(const chemin & path, const request & req)
{
    string ret = "";
    string select_id = get_path().namify();
    const vector<record> & choices = get_choices();
    vector<record>::const_iterator it = choices.begin();

	// for POST method only, extract used choice from the body of the request
	// and update this object's fields;

    update_field_from_request(req);

	// for any request provide an updated HTML content in response

    ret += "<label for=\"" + select_id + "\">" + x_label + "</label>\n";
    ret += "<select name=\"" + select_id + "\" id=\"" + select_id + "\">\n";
    while(it != get_choices().end())
    {
	ret += "<option value=\"" + it->id + "\"";
	if(it->id == get_selected_id())
	    ret += " selected";
	ret += ">" + it->label + "</option>\n";
	++it;
    }
    ret += "</select>\n";
    ret += "<br />\n";

    return ret;
};
