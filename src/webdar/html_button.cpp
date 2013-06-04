    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_button.hpp"

using namespace std;

const string html_button::action = "click";

html_button::html_button(const std::string & label, const std::string & x_event_name):
    inside("", label)
{
    adopt(&inside);
    adopt(&url_class);
    path_has_changed(); // to set link value of "inside" field

    event_name = x_event_name;
    register_name(event_name);
}

void html_button::set_url_class(const html_url_class & val)
{
    foresake(&url_class);
    url_class = val;
    adopt(&url_class);
    inside.set_class(url_class.get_class_id());
}

string html_button::get_body_part(const chemin & path,
				  const request & req)
{
    chemin target = req.get_uri().get_path();
    string choice;

    ack_visible();
    if(get_visible())
    {
	if(!target.empty())
	{
	    choice = target.back();
	    target.pop_back();
	}
	else
	    choice = "";

	if(target == get_path() && choice == action)
	    act(event_name);

	return html_div::get_body_part(path, req);
    }
}


void html_button::path_has_changed()
{
    chemin tmp = get_path();

    tmp.push_back(action);
    inside.change_url(tmp.display());
    inside.set_class(url_class.get_class_id());
}




