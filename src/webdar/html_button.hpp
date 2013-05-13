#ifndef HTML_BUTTON_HPP
#define HTML_BUTTON_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "body_builder.hpp"
#include "events.hpp"
#include "html_url_class.hpp"
#include "html_url.hpp"
#include "html_div.hpp"

class html_button : public body_builder, public events
{
public:
    html_button(const std::string & label, const std::string & x_event_name);

    html_url_class & change_url_class() { return url_class; };

	// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

    virtual void path_has_changed();

private:
    html_div surround;
    html_url inside;
    std::string event_name;
    html_url_class url_class;

    static const std::string action;
};

#endif
