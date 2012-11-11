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


void html_table::add_in_next_cell(body_builder *obj)
{
    bundle bdl;

    if(obj == NULL)
	throw WEBDAR_BUG;
    bdl.name = record_child(obj);
    bdl.obj = obj;
    bdl.static_text = "";
    current_line.push_back(bdl);
    if(current_line.size() == dim_x)
    {
	table.push_back(current_line);
	current_line.clear();
    }
}

void html_table::add_in_next_cell(const string & fixed)
{
    bundle bdl;

    bdl.name = "";
    bdl.obj = NULL;
    bdl.static_text = fixed;
    current_line.push_back(bdl);
    if(current_line.size() == dim_x)
    {
	table.push_back(current_line);
	current_line.clear();
    }
}

const body_builder *html_table::access_cell(unsigned int x, unsigned int y) const
{
    if(y < table.size())
	if(x < dim_x)
	    return table[y][x].obj;
	else
	    throw exception_range("requested cell from table is out of range");
    else
	if(y == table.size())
	    if(x < current_line.size())
		return current_line[x].obj;
	    else
		throw exception_range("requested cell from table is out of range");
	else
	    throw exception_range("requested cell from table is out of range");
}

void html_table::clear()
{
    vector<bundle>::iterator it = current_line.begin();
    vector< vector<bundle> >::iterator tit = table.begin();

    while(it != current_line.end())
    {
	if(it->obj != NULL)
	{
	    unrecord_child(it->obj);
	    delete it->obj;
	    it->obj = NULL;
	}
	++it;
    }
    current_line.clear();

    while(tit != table.end())
    {
	it = tit->begin();
	while(it != tit->end())
	{
	    if(it->obj != NULL)
	    {
		unrecord_child(it->obj);
		delete it->obj;
		it->obj = NULL;
	    }
	    ++it;
	}

	++tit;
    }
    table.clear();
}


string html_table::get_body_part(const chemin & path,
				 const request & req)
{
	// if the last raw is not completed it is now taken into account
	// (in other words, we don't look at current_line in this method

    string ret = "";
    chemin sub_path = path;
    vector< vector<bundle> >::const_iterator it = table.begin();

    if(sub_path.size() > 0)
	sub_path.pop_front();

    ret = "<table " + get_css_string() + " " + border + ">\n";
    while(it != table.end())
    {
	vector<bundle>::const_iterator cell = it->begin();
	ret += "<tr>\n";

	while(cell != it->end())
	{
	    ret += "<td>";
	    if(cell->obj != NULL)
		ret += cell->obj->get_body_part(sub_path, req);
	    else
		ret += cell->static_text;
	    ret += "</td>\n";
	    ++cell;
	}
	ret += "</tr>\n";
	++it;
    }
    ret += "</table>\n";

    return ret;
}
