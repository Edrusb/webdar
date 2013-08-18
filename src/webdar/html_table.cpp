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

html_table::html_table(unsigned int width)
{
    dim_x = width;
    border_collapsed = true;
    cells_title_set = false;
}

void html_table::css_border_collapsed(bool mode)
{
    border_collapsed = string(" border-collapse: ") + (mode ? "collapse;" : "separate;");
}

string html_table::get_body_part(const chemin & path,
				 const request & req)
{
    string ret = "";
    bundle bdl;
    chemin sub_path = path;
    unsigned int line_length = 0;
    bool first_line = true;
    string cell_css_style = build_style();
    string table_css = css_get_raw_string() + border_collapsed;

    if(table_css != "")
	table_css = "style=\"" + table_css + "\"";

    if(sub_path.size() > 0)
	sub_path.pop_front();

    ret = cell_css_style;
    ret += "<table " + table_css  + ">\n";

    reset_read_next();
    while(read_next(bdl))
    {
	    /// the <tr> mark for each new line
	if(line_length == 0)
	    ret += "<tr>\n";

	    /// the <td> ... cell ... </td>
	ret += "<td";
	if(cell_css_style != "")
	{
	    ret += " class=\"";

	    if(first_line && cells_title_set)
		ret += get_class_id(true);
	    else
		ret += get_class_id(false);
	    ret += "\"";
	}
	ret += ">\n";
	if(bdl.obj != NULL)
	    ret += bdl.obj->get_body_part(sub_path, req);
	else
	    ret += bdl.static_text;
	ret += "</td>\n";

	    /// counting cell to known when to close and start a new raw

	++line_length;
	if(line_length == dim_x)
	{
	    ret += "</tr>\n"; // end of raw
	    line_length = 0;
	    first_line = false;
	}
    }

	/// adding empty cells if necessary (well, ... that's a bit useless, but we can't know whether a particular browser will not cry on that)

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

string html_table::get_class_id(bool title) const
{
    return get_path().namify() + (title ? "T" : "A");
}

string html_table::build_style() const
{
    string ret = "";
    string tmp;

	// class for title cells
    if(cells_title_set)
    {
	tmp = cells_title.css_get_raw_string();
	if(tmp != "")
	    ret += "td." + get_class_id(true) + " { " + tmp + "}\n";
    }
	// class for any
    tmp = cells.css_get_raw_string();
    if(tmp != "")
	ret += "td." + get_class_id(false)+ " { " + tmp + "}\n";

    if(ret != "")
	ret = "<style type=\"text/css\">\n" + ret + "</style>\n";

    return ret;
}
