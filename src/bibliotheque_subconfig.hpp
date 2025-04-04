/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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

#ifndef BIBLIOTHEQUE_SUBCONFIG_HPP
#define BIBLIOTHEQUE_SUBCONFIG_HPP

#include "my_config.h"

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "bibliotheque.hpp"

    /// class bibliotheque_subconfig is an interface (pure virtual class)

    /// jsoner objects added to a bibliotheque *MAY* implement this interface in addition to jsoner
    /// when their configuration is subject to depend on other configurations from the same bibliotheque
    /// they are about to be added to (bibliotheque::add_config) or updated in (bibliotheque::update_config).
    /// the class arriere_boutique checks whether this interface is implemented or not and managed this
    /// transparently.

class bibliotheque_subconfig
{
public:

	/// provide a standard mean for an object to tell its configuration relies on other configuration(s)
    virtual bibliotheque::using_set get_using_set() const = 0;

protected:

	/// helper routing for objects of inheriting of that class
    static void subconfig_add_to(bibliotheque::using_set & added_to, const bibliotheque::using_set & to_add);
};

#endif
