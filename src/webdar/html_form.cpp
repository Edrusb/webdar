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

    ret += "<form method=\"post\" action=\"" + get_path().display() + "\" " + get_css_string() + ">\n";
    ret += get_body_part_from_all_children(path, req);
    ret += "<input type=\"submit\" value=\"" + go_mesg + "\" />\n";
    ret += "</form>\n";

    return ret;
}
