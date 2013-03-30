#ifndef HTML_FORM_HPP
#define HTML_FORM_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"


class html_form : public body_builder
{
public:
    html_form(const std::string & validate_msg = "send") { go_mesg = validate_msg; visible = true; };

	/// inherited methods from body_builder

    virtual std::string get_body_part(const chemin & path,
				      const request & req);

    void set_visible(bool val) { visible = val; };

private:
    bool visible;
    std::string go_mesg;

};

#endif
