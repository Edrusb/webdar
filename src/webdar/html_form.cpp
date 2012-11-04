    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_form.hpp"

using namespace std;

html_form::html_form(const std::string & target,
		     const std::string & validate_msg)
{
    url = target;
    fields = "";
    go_mesg = validate_msg;
}

string html_form::display() const
{
    string ret = "";

    ret += "<form method=\"post\" action=\"" + url + "\" " + get_css_string() + ">\n";
    ret += fields;
    ret += "<input type=\"submit\" value=\"" + go_mesg + "\" />\n";
    ret += "</form>\n";

    return ret;
}
