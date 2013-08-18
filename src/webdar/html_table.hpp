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
#include "html_level.hpp"
#include "webdar_tools.hpp"
#include "exceptions.hpp"

class html_table: public html_level
{
public:
    html_table(unsigned int width);
    html_table(const html_table & ref) { throw WEBDAR_BUG; };
    const html_table & operator = (const html_table & ref) { throw WEBDAR_BUG; };
    ~html_table() { clear(); };

	/// whether border are collapsed or separated (collasped by default)
    void css_border_collapsed(bool mode);

	/// define a specific css object for the _cells_ first raw
    void set_css_cells_first_raw(const css & val) { cells_title_set = true; cells_title = val; };

	/// clear apreviously assigned css object to the title raw
    void set_css_cells_first_raw() { cells_title_set = false; cells_title.css_clear_attributes(); };

	/// assigned to all celles except those of the first raw if set_css_cells_first_raw was set
    void set_css_cells(const css & val) { cells = val; };

	/// clear apreviously assigned css object to the title raw
    void set_css_cells() { cells.css_clear_attributes(); };

	/// assign to all cells

    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    unsigned int dim_x;
    std::string border_collapsed;
    bool cells_title_set;
    css cells_title;
    css cells;

    std::string get_class_id(bool title) const;
    std::string build_style() const;


};

#endif
