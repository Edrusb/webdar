#ifndef HTML_TABLE_HPP
#define HTML_TABLE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <vector>
#include <string>

    // webdar headers
#include "html.hpp"
#include "webdar_tools.hpp"

class html_table: public html
{
public:
    html_table(unsigned int width) { dim_x = width; border = ""; table.clear(); current_line.clear(); };

	/// add a new cell to the table
	///
	/// \note before starting a new row, the css attributes are saved
	/// and will be set for this raw
	/// when calling display(), the css attributes are applied for the whole table
    void add_cell(const std::string & val);

    void set_border(unsigned int width) { border = "border=\"" + webdar_tools_convert_to_string(width) + "\""; };

    virtual std::string display() const;

private:
    unsigned int dim_x;
    std::string border;
    std::vector<std::string> current_line;
    std::vector<std::string> css_per_line;
    std::vector< std::vector<std::string> >table;

};




#endif
