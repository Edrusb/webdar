#ifndef MENU_HPP
#define MENU_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <map>

    // webdar headers
#include "body_builder.hpp"
#include "html_url.hpp"
#include "html_div.hpp"
#include "html_url_class.hpp"

class menu : public body_builder
{
public:
    menu();

	/// add an entry in the menu
	///
	/// \param[in] reference must be a single word, letters only,
	/// lower case and different from any other reference already give so far
	/// \param[in] label is any text that will be showed to the user
    void add_entry(const std::string & reference, const std::string & label);

	/// return the reference of the current mode
	/// \return the selected item in the order of addition using add_entry
	/// starting from 1: First add_entry gives item number 1
    unsigned int get_current_mode() const { return current_mode + 1; };

	/// inherited from body_builder
    std::string get_body_part(const chemin & path,
			      const request & req);


protected:
	/// inherited from body_builder
    void path_has_changed();

	/// inherited from css (grand-parent class)
    void css_updated(bool inherit);

private:
    struct boite
    {
	std::string value;
	html_div *surround;
	html_url *inside;
	boite() { surround = NULL; inside = NULL; };
    };

    unsigned int current_mode; // which item is currently selected
    html_div *global;    // managed by body_builder parent class, but direct access needed
    std::vector<boite> item;
    html_div box_off; //< used to assign CSS attributes: unselected item
    html_div box_on;  //< used to assign CSS attributes: selected item
    html_div box_void;//< used to assign CSS attributes: separators
    html_url_class *url_selected; //< links when box selected
    html_url_class *url_normal;   //< links when box no selected

	/// set html field accordingly to the new mode
	/// \param[in] mode is the mode to set, it start from 0 !!!
    void set_mode(unsigned int mode);
};

#endif
