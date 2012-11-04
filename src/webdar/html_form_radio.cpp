    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_form_radio.hpp"

using namespace std;

void html_form_radio::add_choice(const string & id, const string & label)
{
    record x;
    x.id = id;
    x.label = label;
    choices.push_back(x);
}


string html_form_radio::display() const
{
    string ret = "";
    vector<record>::const_iterator it = choices.begin();

    while(it != choices.end())
    {
	ret += "<input type=\"radio\" name=\"" + get_id() + "\" id=\"" + it->id + "\" />\n";
	ret += "<label for=\"" + it->id + "\">" + it->label + "</label><br />\n";
	++it;
    }

    return ret;
}
