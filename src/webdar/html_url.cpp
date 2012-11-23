    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_url.hpp"

using namespace std;


string html_url::get_body_part() const
{
    return string("<a ") + css_get_string() + x_class + " href=\"" + x_url + "\">" + x_label + "</a>";
}
