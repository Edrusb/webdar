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

class html_size_unit : public body_builder, public actor
{
public:
    html_size_unit();

    libdar::infinint get_value() const;

	/// body_builder indirect inheritance
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// actor inheritance
    virtual void on_event(const std::string & event_name);

private:
    html_form_select unit;
    html_form_select SI_mode;
    bool changed;


    void set_fields();
};

#endif
