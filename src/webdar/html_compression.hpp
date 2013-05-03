#ifndef HTML_COMPRESSION_HPP
#define HTML_COMPRESSION_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <string>

    // webdar headers

#include "html_form_select.hpp"

class html_compression : public html_form_select
{
public:
    static const std::string changed;

    html_compression(const std::string & title);

    libdar::compression get_value() const;
    void set_value(libdar::compression val);

	/// actor inheritance
    virtual void on_event(const std::string & event_name);
};

#endif
