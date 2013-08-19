/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
//
// This file is part of Webdar
//
//  Webdar is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Webdar is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
//
//----
//  to contact the author: dar.linux@free.fr
/*********************************************************************/

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
    ~html_button() { if(url_class != NULL) { foresake(url_class); delete url_class; } };

    void set_url_classid(const std::string & classid) { inside.set_class(classid); };
    void set_url_class(const html_url_class & val);
    void set_url_css(const css & val) { inside.css_inherit_from(val); };
    void change_label(const std::string & label) { inside.change_label(label); };
    std::string get_label() const { return inside.get_label(); };

	// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

    virtual void path_has_changed();

private:
    html_url inside;
    std::string event_name;
    html_url_class *url_class;

    static const std::string action;
};

#endif
