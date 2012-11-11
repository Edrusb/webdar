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
#include "body_builder.hpp"
#include "webdar_tools.hpp"
#include "exceptions.hpp"

class html_table: public body_builder
{
public:
    html_table(unsigned int width) { dim_x = width; border = ""; };
    html_table(const html_table & ref) { throw WEBDAR_BUG; };
    const html_table & operator = (const html_table & ref) { throw WEBDAR_BUG; };
    ~html_table() { clear(); };

	/// add the object in the next new cell of the table
	///
	/// \param[in] obj is the address of the object that will be asked to fill the cell
	/// \note, the object which address become owned by the table. It will be destroyed
	/// by the table when no more necessary.
    void add_in_next_cell(body_builder *obj);
    void add_in_next_cell(const std::string & fixed);

	/// get access to the object filling a given cell of the table
	///
	/// \note the object which address is returned stays under the responsibility of the table
    const body_builder *access_cell(unsigned int x, unsigned int y) const;

    void set_border(unsigned int width) { border = "border=\"" + webdar_tools_convert_to_string(width) + "\""; };

	/// clear all the cells of the table, destroying all associated objects
    void clear();

    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    struct bundle
    {
	std::string static_text; //< if different than "" obj field is not used
	body_builder *obj;       //< if set to NULL, using static_text instead
	std::string name;
    };

    unsigned int dim_x;
    std::string border;
    std::vector<bundle> current_line;
    std::vector< std::vector<bundle> >table;

};

#endif
