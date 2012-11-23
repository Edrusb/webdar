    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_image.hpp"

using namespace std;

html_image::html_image(const std::string & url, const std::string & alt)
{
    x_url = url;
    x_alt = alt;
    dim_x = "";
    dim_y = "";
}

string html_image::get_body_part() const
{
    string ret;

    ret = "<img " + css_get_string() + " ";
    ret += "src=\"" + x_url + "\" alt=\"" + x_alt +"\" ";
    if(dim_x != "" && dim_y != "") // yes both
	ret += "width=\"" + dim_x + "\" heigth=\"" + dim_y + "\" ";

    return ret;
}
