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

#ifndef html_bibliotheque_HPP
#define html_bibliotheque_HPP

    // C system header files
#include "my_config.h"

extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "body_builder.hpp"
#include "html_tabs.hpp"
#include "bibliotheque.hpp"
#include "arriere_boutique.hpp"
#include "html_entrepot.hpp"

    /// class html_bibliotheque provide graphical mean to manipulate a bibliotheque object

    /// class html_bibliotheque let user (thanks to its web interface) to:
    /// - define, update, remove components to the bibliotheque (per category)
    /// - save/load, upload/download the whole bibliotheque to file or throw the http(s) channel

class html_bibliotheque: public body_builder
{
public:
    html_bibliotheque(std::shared_ptr<bibliotheque> & ptr);
    html_bibliotheque(const html_bibliotheque & ref) = default;
    html_bibliotheque(html_bibliotheque && ref) noexcept = default;
    html_bibliotheque & operator = (const html_bibliotheque & ref) = default;
    html_bibliotheque & operator = (html_bibliotheque && ref) noexcept = default;
    ~html_bibliotheque() = default;

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;


private:
    std::shared_ptr<bibliotheque> biblio;

    std::unique_ptr<arriere_boutique<html_entrepot> > ab_entrepot;

    html_tabs tabs;


    static constexpr const char* tab_main = "main";
    static constexpr const char* tab_repo = "repo";
};

#endif
