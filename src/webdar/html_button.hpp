#ifndef HTML_BUTTON_HPP
#define HTML_BUTTON_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "events.hpp"
#include "html_url_class.hpp"
#include "html_url.hpp"
#include "html_div.hpp"

class html_button : public html_div, public events
{
public:
    html_button(const std::string & label, const std::string & x_event_name);

    void set_url_class(const html_url_class & val);
    void set_url_css(const css & val) { inside.css_inherit_from(val); };

	// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

    virtual void path_has_changed();

private:
    html_url inside;
    std::string event_name;
    html_url_class url_class;

    static const std::string action;
};

#endif
