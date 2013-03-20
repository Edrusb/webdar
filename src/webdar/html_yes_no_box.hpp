#ifndef HTML_YES_NO_BOX_HPP
#define HTML_YES_NO_BOX_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "exceptions.hpp"
#include "body_builder.hpp"
#include "html_form.hpp"
#include "html_form_radio.hpp"

class html_yes_no_box : public body_builder
{
public:
    html_yes_no_box(const std::string & message, bool default_value);

    bool get_value() const { if(rd == NULL) throw WEBDAR_BUG; return rd->get_selected_num() != 0; };

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req)
    { return form->get_body_part(path, req); };

protected:
	/// inherited from body_builder
    virtual void inherited_give(body_builder *obj) { if(obj == NULL) throw WEBDAR_BUG; (void)record_child(obj); };


private:
    html_form *form;     // a pointer to a value managed by parent class
    html_form_radio *rd; // another  pointer to a value managed by parent class

};

#endif
