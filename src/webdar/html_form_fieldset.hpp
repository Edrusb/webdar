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
    void clear() { clear_and_delete_children(); };

    virtual std::string get_body_part(const chemin & path,
				      const request & req);

protected:
    	/// add this object as member of the fieldset
	///
	/// \note this call transfer memory management responsibility to the fieldset
    void inherited_give(body_builder *obj) { (void)record_child(obj); };

private:
    std::string x_label;

};


#endif
