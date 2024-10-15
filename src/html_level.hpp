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

#ifndef HTML_LEVEL_HPP
#define HTML_LEVEL_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>
#include <list>

    // webdar headers
#include "body_builder.hpp"

    /// class html_level is a pure virtual class
    ///
    /// it implements the management of a mixed list of object and static html text
    /// and provides method to read this list. This class does not implement the
    /// get_body_part() pure virtual method of its body_builder ancestor class
    /// this is left to its inherited classes

class html_level : public body_builder
{
public:
    html_level();
    html_level(const html_level & ref) = delete;
    html_level(html_level && ref) noexcept = delete;
    html_level & operator = (const html_level & ref) = delete;
    html_level & operator = (html_level && ref) noexcept = delete;
    ~html_level() = default;

	/// used in concurrency with body_builder::adopt()
    void adopt_static_html(const std::string & html);

	/// clear all adopted data
    void clear_children();

protected:
	/// inherited from body_builder
    virtual void has_adopted(body_builder *obj) override;

	/// inherited from body_builder
    virtual void will_foresake(body_builder *obj) override;

	/// struct recording the content of each level (dynamic or static html component)
    struct bundle
    {
	body_builder* obj;       //< if set to nullptr, using static_text instead
	std::string static_text; //< used if obj is nullptr
	bundle() { obj = nullptr; };
    };

    void reset_read_next() { nxt = table.begin(); };
    bool read_next(bundle & bdl) const; ///< return false if bdl could not be set (nor more entry to read)

	/// this is an alternative of using read_next, it creates the body from both static and body_builder objects
	///
	/// \note to be used when all children body_part are needed as a single block of text
    std::string get_body_part_from_children_as_a_block(const chemin & path,
						       const request & req);

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    mutable std::list<bundle>::iterator nxt; /// < used by read_next()
    std::list<bundle> table;
};

#endif
