#ifndef HTML_FOCUS_HPP
#define HTML_FOCUS_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "body_builder.hpp"

    /// html_focus class provides a mean to temporarily have an object show at a particular place in a page
    ///
    /// the html_focus is adopted as any body_builder object but does not show anything
    /// until a body_builder object is given_for_temporary_adoption(). Any previous object
    /// given that way are foresaken and replaced by the new one.

class html_focus: public body_builder
{
public:
    html_focus() { adopted = NULL; };

	/// define what to display under the focus
	///
	/// \param[in] obj is the object to be displayed
	/// \note object given stay under responsbility of the caller for memory management
    void given_for_temporary_adoption(body_builder *obj);

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

protected:
    virtual void has_been_foresaken(body_builder *obj) { if(obj == adopted) adopted = NULL; };

private:
    body_builder *adopted; //< the object thas has been temporarily adopted
};





#endif
