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
    html_form(const std::string & validate_msg = "send") { go_mesg = validate_msg; };

	/// only record the given object as child, no memory management is given about that object
    void add_field(body_builder *obj) { (void)record_child(obj); };

    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    std::string go_mesg;
};

#endif
