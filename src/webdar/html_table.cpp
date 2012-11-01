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


void html_table::add_cell(const std::string & val)
{
    if(current_line.size() == 0)
	css_per_line.push_back(get_css_string());
    current_line.push_back(val);
    if(current_line.size() == dim_x)
    {
	table.push_back(current_line);
	current_line.clear();
    }
}

string html_table::display() const
{
    string ret = "";
    vector< vector<string> >::const_iterator it = table.begin();
    vector<string>::const_iterator css = css_per_line.begin();

    while(current_line.size() != 0)
	const_cast<html_table *>(this)->add_cell("");

    ret = "<table " + get_css_string() + " " + border + ">\n";
    while(it != table.end())
    {
	vector<string>::const_iterator cell = it->begin();
	if(css != css_per_line.end())
	{
	    ret += "<tr " + *css + ">\n";
	    ++css;
	}
	else
	    throw WEBDAR_BUG;

	while(cell != it->end())
	{
	    ret += "<td>" + *cell + "</td>\n";
	    ++cell;
	}
	ret += "</tr>\n";
	++it;
    }
    ret += "</table>\n";

    return ret;
}


