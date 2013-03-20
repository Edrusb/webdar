#ifndef HTML_FORM_FIELDSET_HPP
#define HTML_FORM_FIELDSET_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"

class html_form_fieldset : public body_builder
{
public:
    html_form_fieldset(const std::string & label) { x_label = label; };

	/// clear and destroy previously added objects
    void clear() { orphan_all_children(); };

    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    std::string x_label;

};

#endif
