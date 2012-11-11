    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "css.hpp"

using namespace std;


void css::clear_css_attributes()
{
    color = bg_col = bg_img = "";
    img_pos_x = img_pos_y = "";
}

string css::get_css_string() const
{
    string ret;

    if(color != "")
	ret += " color: " + color + ";";
    if(bg_col != "")
	ret += " background-color: "  + bg_col + ";";
    if(bg_img != "")
    {
	string bg_repeat = " background-repeat: ";

	ret += " background-image: url(\"" + bg_img + "\");";
	if(img_x)
	    if(img_y)
		ret += bg_repeat + "repeat;";
	    else
		ret += bg_repeat + "repeat-x;";
	else
	    if(img_y)
		ret += bg_repeat + "repeat-y;";
	    else
		    ret += bg_repeat + "no-repeat;";

	ret += string(" background-attachment: ") + (img_fixed ? "fixed;" : "scroll;");
    }
    if(img_pos_x != "" && img_pos_y != "") // yes both
	ret += string(" background-position: ") + img_pos_x + " " + img_pos_y + ";";

    return ret;
}

