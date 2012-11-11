    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_form_fieldset.hpp"

using namespace std;


string html_form_fieldset::get_body_part(const chemin & path,
		     const request & req)
{
    string ret = "";

    ret += "<fieldset " + get_css_string() + ">\n";
    ret += "<legend>" + x_label + "</legend>\n";
    ret += get_body_part_from_all_children(path, req);
    ret += "</fieldset>\n";
    return ret;
}
