    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_form_select.hpp"

using namespace std;

string html_form_select::display() const
{
    string ret = "";
    vector<record>::const_iterator it = get_choices().begin();

    ret += "<label for=\"" + get_id() + "\">" + x_label + "</label><br />\n";
    ret += "<select name=\"" + get_id() + "\" id=\"" + get_id() + "\">\n";
    while(it != get_choices().end())
    {
	ret += "<option value=\"" + it->id + "\">" + it->label + "</option>\n";
	++it;
    }
    ret += "</select>\n";
    return ret;
};
