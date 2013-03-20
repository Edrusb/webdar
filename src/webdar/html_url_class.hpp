#ifndef HTML_URL_CLASS_HPP
#define HTML_URL_CLASS_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "body_builder.hpp"


class html_url_class : public body_builder
{
public:
    std::string get_class_id() const { return std::string(" class=\"") + get_path().namify() + "\""; };

    void set_style_link(const css & ref) { link = ref.css_get_raw_string(); };
    void set_style_visited(const css & ref) { visited = ref.css_get_raw_string(); };
    void set_style_active(const css & ref) { active = ref.css_get_raw_string(); };
    void set_style_hover(const css & ref) { hover = ref.css_get_raw_string(); };

	/// inherited from body_builder
    std::string get_body_part(const chemin & path,
			      const request & req);

private:
    std::string link;
    std::string visited;
    std::string active;
    std::string hover;
};


#endif
