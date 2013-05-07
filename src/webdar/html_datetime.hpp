#ifndef HTML_DATETIME_HPP
#define HTML_DATETIME_HPP

    // C system header files
extern "C"
{
#include <time.h>
}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_form_input.hpp"
#include "html_form_select.hpp"


class html_datetime : public body_builder
{
public:
    html_datetime(const std::string & title);

    libdar::infinint get_value() const;
    void set_value(libdar::infinint val);

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    html_form_input year;
    html_form_select month;
    html_form_select day;
    html_form_select hour;
    html_form_select minute;

    void update_from(const struct tm & val);
};




#endif
