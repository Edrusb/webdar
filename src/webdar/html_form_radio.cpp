    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_form_radio.hpp"

using namespace std;

const string html_form_radio::changed = "html_form_radio::changed";

void html_form_radio::add_choice(const string & id, const string & label)
{
    record x;
    x.id = id;
    x.label = label;
    choices.push_back(x);
}

void html_form_radio::set_selected(unsigned int x)
{
    if(x >= choices.size())
	selected = choices.size() - 1;
    else
	selected = x;
}

string html_form_radio::get_body_part(const chemin & path,
				      const request & req)
{
    string ret = "";
    string radio_id = get_path().namify();

	// for POST method only, extract user choice from the body of the request
	// and update this object's fields

    if(get_visible())
	update_field_from_request(req);

	// for any request provide an updated HMTL content in response

    if(get_next_visible())
    {
	for(unsigned int i = 0; i < choices.size(); ++i)
	{
	    ret += "<input " + css_get_string() + " type=\"radio\" name=\"" + radio_id + "\" id=\"" + choices[i].id + "\" value=\"" + choices[i].id + "\" ";
	    if(i == selected)
		ret += "checked ";
	    ret += "/>\n";
	    ret += "<label " + css_get_string() + " for=\"" + choices[i].id + "\">" + choices[i].label + "</label>";
	    if(i+1 < choices.size() || !get_no_CR())
		ret += "<br />\n";
	    else
		ret += "\n";
	}
    }
    else
	ret = "";

    ack_visible();

    return ret;
}

void html_form_radio::update_field_from_request(const request & req)
{
    if(req.get_method() == "POST")
    {
	map<string, string> bd = req.get_body_form();
	map<string, string>::const_iterator it = bd.find(get_path().namify());
	if(it != bd.end())
	{
	    unsigned int u = 0;
	    while(u < choices.size() && choices[u].id != it->second)
		++u;
	    if(u < choices.size())
	    {
		bool has_changed = selected != u;

		selected = u;
		if(has_changed)
		    act(changed);
	    }
	}
    }
}
