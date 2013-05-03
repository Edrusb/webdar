#ifndef HTML_SIZE_UNIT_HPP
#define HTML_SIZE_UNIT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "html_form_select.hpp"

class html_size_unit : public html_form_select
{
public:
    html_size_unit();

    libdar::infinint get_value() const;

	/// actor indirect inheritance
    virtual void on_event(const std::string & event_name);

	/// body_builder indirect inheritance
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    html_form_select SI_mode;

    void set_fields();
};

#endif
