#ifndef HTML_COMPARISON_FIELDS_HPP
#define HTML_COMPARISON_FIELDS_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <vector>
#include <string>
#include <dar/libdar.hpp>

    // webdar headers
#include "html_form_select.hpp"

class html_comparison_fields : public html_form_select
{
public:
    html_comparison_fields();

    libdar::inode::comparison_fields get_value() const;
    void set_value(libdar::inode::comparison_fields val);
};




#endif
