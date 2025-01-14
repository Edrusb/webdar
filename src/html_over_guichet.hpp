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

#ifndef HTML_OVER_GUICHET_HPP
#define HTML_OVER_GUICHET_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "html_mask.hpp"
#include "jsoner.hpp"
#include "bibliotheque_subconfig.hpp"
#include "guichet.hpp"
#include "html_form_dynamic_table.hpp"
#include "html_mask_form_filename.hpp"

class html_over_guichet : public guichet,
			  public html_mask
{
public:
	/// constructor
    html_over_guichet();

    	/// mandatory call to provide the object to adopt and use to load/save json and get mask configurations

	/// \param[in] ptr pointer to a bibliotheque where from to fetch configurations
	/// \param[in] cat category in which to look for named in the bibliotheque
	/// \param[in] to_give object of the type corresponding to the category be given to this html_over_guichet
	/// \param[in] add_form_around whether a form around to_give is necessary for proper functionning
	/// \note to_give must also be a jsoner and a html_mask, it pass under the managment responsibility of the
	/// html_over_guichet object.
    void set_child(const std::shared_ptr<bibliotheque> & ptr,
		   std::unique_ptr<html_mask_form_filename> & to_give,
		   bibliotheque::category cat);

	/// inherited from html_mask
    virtual std::unique_ptr<libdar::mask> get_mask() const override;

	/// inherited_from jsoner
    virtual void load_json(const json & source) override;

    	/// inherited_from jsoner
    virtual json save_json() const override;

    	/// inherited_from jsoner
    virtual void clear_json() override;

	/// inherited from bibliotheque_subconfig
    virtual bibliotheque::using_set get_using_set() const override;

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;


private:
    guichet wicket;
    std::shared_ptr<html_mask_form_filename> inner;

    void check_inner() const { if(!inner) throw WEBDAR_BUG; };
};


#endif
