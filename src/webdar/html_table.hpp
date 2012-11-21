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
    html_table(unsigned int width) { dim_x = width; border = ""; };
    html_table(const html_table & ref) { throw WEBDAR_BUG; };
    const html_table & operator = (const html_table & ref) { throw WEBDAR_BUG; };
    ~html_table() { clear(); };

    void set_border(unsigned int width) { border = "border=\"" + webdar_tools_convert_to_string(width) + "\""; };

    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    unsigned int dim_x;
    std::string border;

};

#endif
