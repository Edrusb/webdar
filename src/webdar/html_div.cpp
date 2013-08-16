    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_div.hpp"

using namespace std;


string html_div::get_body_part(const chemin & path,
			       const request & req)
{
    string ret = "<div " + css_get_string() + ">\n";

    if(get_visible() || get_next_visible())
    {
	ret += get_body_part_from_children_as_a_block(path, req);
	ret += "\n</div>\n";
	if(!get_no_CR())
	    ret += "<br />\n";
    }
    ack_visible();
    if(!get_visible())
	ret = "";

    return ret;
}
