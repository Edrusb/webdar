    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "exceptions.hpp"


    //
#include "html_table.hpp"

using namespace std;


string html_table::get_body_part(const chemin & path,
				 const request & req)
{
    string ret = "";
    bundle bdl;
    chemin sub_path = path;
    unsigned int line_length = 0;

    if(sub_path.size() > 0)
	sub_path.pop_front();

    ret = "<table " + get_css_string() + " " + border + ">\n";
    reset_read_next();
    while(read_next(bdl))
    {
	if(line_length == 0)
	    ret += "<tr>\n";
	ret += "<td>\n";
	if(bdl.obj != NULL)
	    ret += bdl.obj->get_body_part(sub_path, req);
	else
	    ret += bdl.static_text;
	ret += "</td>\n";

	++line_length;
	if(line_length == dim_x)
	{
	    ret += "</tr>\n";
	    line_length = 0;
	}
    }

    if(line_length != 0)
    {
	while(line_length < dim_x)
	{
	    ret += "<td></td>\n";
	    ++line_length;
	}
	ret += "<tr>\n";
    }
    ret += "</table>\n";

    return ret;
}
