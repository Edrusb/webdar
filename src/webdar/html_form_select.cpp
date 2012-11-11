    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_form_select.hpp"

using namespace std;

string html_form_select::get_body_part(const chemin & path, const request & req)
{
    string ret = "";
    string select_id = get_path().namify();
    const vector<record> choices = get_choices();
    vector<record>::const_iterator it = choices.begin();

	// for POST method only, extract used choice from the body of the request
	// and update this object's fields;

    update_field_from_request(req);

	// for any request provide an updated HTML content in response

    ret += "<label for=\"" + select_id + "\">" + x_label + "</label><br />\n";
    ret += "<select name=\"" + select_id + "\" id=\"" + select_id + "\">\n";
    while(it != get_choices().end())
    {
	ret += "<option value=\"" + it->id + "\">" + it->label + "</option>\n";
	++it;
    }
    ret += "</select>\n";

    return ret;
};
