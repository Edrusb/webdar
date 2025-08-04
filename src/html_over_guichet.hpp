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

#ifndef HTML_OVER_GUICHET_HPP
#define HTML_OVER_GUICHET_HPP

#include "my_config.h"

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
#include "events.hpp"
#include "actor.hpp"

    /// class html_over_guichet is used to insert configuration from a bibliotheque in-place of a subconfiguration

    /// this allows configuration in a bibliotheque to rely on some other configurations from the same bibliotheque store.
    /// It is used for file and mask filtering mainly.

class html_over_guichet : public body_builder,
			  public html_mask,
			  public jsoner,
			  public bibliotheque_subconfig,
			  public actor,
			  public events
{
public:

    static const std::string changed;

	/// constructor
    html_over_guichet();

    html_over_guichet(const html_over_guichet & ref) = delete;
    html_over_guichet(html_over_guichet && ref) noexcept = delete;
    html_over_guichet & operator = (const html_over_guichet & ref) = delete;
    html_over_guichet & operator = (html_over_guichet && ref) noexcept = delete;
    ~html_over_guichet() = default;

    	/// mandatory call to provide the object to adopt and use to load/save json and get mask configurations

	/// \param[in] ptr pointer to a bibliotheque where from to fetch configurations
	/// \param[in] to_give object of the type corresponding to the category be given to this html_over_guichet
	/// \param[in] cat category in which to look for named in the bibliotheque
	/// \param[in] changed_event if not an empty string lead the over_guichet object to record itself on the to_give
	/// pointed-to object for the provided changed_event, upon which it will trigger its own changed event up change
	/// of the to_give object
	/// \note to_give must also be a jsoner and a html_mask, it pass under the managment responsibility of the
	/// html_over_guichet object.
    void set_child(const std::shared_ptr<bibliotheque> & ptr,
		   std::unique_ptr<body_builder> & to_give,
		   bibliotheque::category cat,
		   const std::string & changed_event);

	/// inherited from html_mask
    virtual std::unique_ptr<libdar::mask> get_mask() const override;

	/// inherited from html_mask
    virtual bool is_relative() const override;


	/// inherited_from jsoner
    virtual void load_json(const json & source) override;

    	/// inherited_from jsoner
    virtual json save_json() const override;

    	/// inherited_from jsoner
    virtual void clear_json() override;

	/// inherited from bibliotheque_subconfig
    virtual bibliotheque::using_set get_using_set() const override;

	/// inherited from actor parent class
    virtual void on_event(const std::string & event_name) override;

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;


private:
    std::string child_event;   ///< used for sanity check in on_event()
    guichet wicket;
    std::shared_ptr<body_builder> inner;

    void check_inner() const { if(!inner) throw WEBDAR_BUG; };
};


#endif
