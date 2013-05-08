    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_form.hpp"

using namespace std;

string html_form::get_body_part(const chemin  & path,
				const request & req)
{
    string ret = "";

    ret += "<form method=\"post\" action=\"" + get_path().display() + "\">\n";
    if(get_path() != req.get_uri().get_path() && req.get_method() == "POST" || !get_visible() && get_next_visible())
    {
	request tmp = req;
	tmp.change_method("GET");
	ret += get_body_part_from_all_children(path, tmp);
    }
    else
	ret += get_body_part_from_all_children(path, req);
    ret += "<input " + css_get_string() + " type=\"submit\" value=\"" + go_mesg + "\" />\n";
    ret += "</form>\n";

    if(!get_next_visible())
	ret = "";
    ack_visible();

    return ret;
}
