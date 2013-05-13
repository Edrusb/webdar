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
    surround.adopt(&url_class);
    surround.adopt(&inside);
    adopt(&surround);
    path_has_changed(); // to set link value of "inside" field

    event_name = x_event_name;
    register_name(event_name);
}

string html_button::get_body_part(const chemin & path,
				  const request & req)
{
    chemin target = req.get_uri().get_path();
    string choice;

    if(!target.empty())
    {
	choice = target.back();
	target.pop_back();
    }
    else
	choice = "";

    if(target == get_path() && choice == action)
	act(event_name);

    return get_body_part_from_all_children(path, req);
}


void html_button::path_has_changed()
{
    chemin tmp = get_path();

    tmp.push_back(action);
    inside.change_url(tmp.display());
}




