    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_form_fieldset.hpp"

using namespace std;


string html_form_fieldset::display() const
{
    string ret = "";

    ret += "<fieldset " + get_css_string() + ">\n";
    ret += "<legend>" + x_label + "</legend>\n";
    ret += fields;
    ret += "</fieldset>\n";
    return ret;
}
