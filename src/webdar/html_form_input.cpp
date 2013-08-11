    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "webdar_tools.hpp"
#include "exceptions.hpp"


    //
#include "html_form_input.hpp"

using namespace std;

const string html_form_input::changed = "html_form_input_changed";

html_form_input::html_form_input(const std::string & label,
				 input_type type,
				 const std::string & initial_value,
				 unsigned int size)
{
    x_label = label;
    x_type = string_for_type(type);
    x_init = initial_value;
    x_size = webdar_tools_convert_to_string(size);
    x_min = x_max = "";
    modif_change = "";
    enabled = true;

    register_name(changed);
}

void html_form_input::set_range(int min, int max)
{
    x_min = webdar_tools_convert_to_string(min);
    x_max = webdar_tools_convert_to_string(max);
}

string html_form_input::get_body_part(const chemin & path,
				      const request & req)
{
    string ret = "";
    string x_id = get_path().namify();

	// first we extract informations from the returned form in
	// the body of the request

    if(req.get_method() == "POST" && path.empty() && get_visible() && enabled)
    {
	string old = x_init;

	map<string, string> fields = req.get_body_form();
	map<string, string>::iterator it = fields.find(x_id);
	if(it != fields.end())
	    x_init = it->second;
	else
	{
	    if(get_visible())
		x_init = "";
	}

	if(x_init != old)
	{
	    act(changed);
	    if(modif_change != "")
		act(modif_change);
		// yes we have to trigger both events,
		// the default, on which some objects might
		// have registered and the modifed one too.
	}
    }

	// we can now return the up to date value of
	// the field

    if(get_next_visible())
    {
	if(x_type != "checkbox")
	    ret += "<label for=\"" + x_id + "\">" + x_label + "</label>\n";
	ret += "<input " + css_get_string() + " type=\"" + x_type + "\" name=\"" + x_id + "\" id=\"" + x_id + "\" ";
	if(x_min != "" && x_max != "") // yes both
	    ret += "min=\"" + x_min + "\" max=\"" + x_max + "\" ";
	if(x_init != "")
	{
	    if(x_type == "checkbox")
		ret += "checked ";
	    else
		ret += "value=\"" + x_init +"\" ";
	}
	if(x_size != "")
	    ret += "size=\"" + x_size + "\" ";
	if(!enabled)
	    ret += "disabled ";
	ret += "/>";
	if(x_type == "checkbox")
	    ret += "<label for=\"" + x_id + "\">" + x_label + "</label>\n";
	if(!get_no_CR())
	    ret += "<br />\n";
    }
    else // HTML control not visible
	ret = "";

    ack_visible();

    return ret;
}

void html_form_input::set_change_event_name(const std::string & name)
{
    if(modif_change != "")
	throw WEBDAR_BUG; // can only be used once, as unregistering an event name is not available
    modif_change = name;
    register_name(modif_change); // this is in addition to the default event name
}

string html_form_input::string_for_type(input_type type)
{
    string ret;

    switch(type)
    {
    case text:
	ret = "text";
	break;
    case password:
	ret = "password";
	break;
    case number:
	ret = "number";
	break;
    case range:
	ret = "range";
	break;
    case check:
	ret = "checkbox";
	break;
    default:
	throw WEBDAR_BUG;
    }

    return ret;
}

