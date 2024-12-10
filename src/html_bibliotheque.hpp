/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2024 Denis Corbin
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

#ifndef HTML_BIBLIOTHEQUE_HPP
#define HTML_BIBLIOTHEQUE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <map>
#include <string>
#include <memory>

    // webdar headers
#include "jsoner.hpp"
#include "body_builder.hpp"
#include "html_text.hpp"
#include "html_tabs.hpp"

    /// \file html_bibliotheque.hpp defines html_bibliotheque class


    /// class html_bibliotheque

    /// class html_bibliotheque purpose is to assign a label per category
    /// to jsoner objects that constitue a given configuration set.
    /// Being itself a jsoner this object is in charge of the global
    /// format of the configuration file, calling in turn the different

class html_bibliotheque : public jsoner, public body_builder
{
public:
	/// json objects are split into category, each used as a different namespace
    enum category { filefilter, pathfilter, command, repo, confsave, conftest, confdiff, conflist, confrest, confmerg, confrepair, confcommon, EOE };
    void begin(category & cat); // reset to first value
    bool end(category cat) { return cat >= EOE; };
    void incr(category & v); // ++cat implementation with out of bound control

    html_bibliotheque();
    html_bibliotheque(const html_bibliotheque & ref) = delete;
    html_bibliotheque(html_bibliotheque && ref) noexcept(false) = delete;
    html_bibliotheque & operator = (const html_bibliotheque & ref) = delete;
    html_bibliotheque & operator = (html_bibliotheque && ref) noexcept(false) = delete;
    virtual ~html_bibliotheque() = default;


	/// inherited from jsoner
    virtual void load_json(const json & source) override {};

    	/// inherited from jsoner
    virtual json save_json() const override { return json(); };


protected:
	/// inheroted from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

    	/// inheroted from body_builder
    virtual void new_css_library_available() override;

private:

	/// structure used to record components stored in the bibliotheque

    html_tabs tabs;
    static constexpr unsigned int numtabs = EOE; // EOE for End Of Enumeration
    html_text text[numtabs];

    static const char* css_tabs;
};

#endif
