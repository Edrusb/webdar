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

#ifndef BIBLIOTHEQUE_HPP
#define BIBLIOTHEQUE_HPP

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

    /// \file bibliotheque.hpp defines bibliotheque class


    /// class bibliotheque

    /// class bibliotheque purpose is associate json configurations
    /// to a name and a category, and provide means to store, search,
    /// update and delete them as a whole. This class provides its own
    /// configuration (including configuration of it stores) as a jsoner
    /// class.

class bibliotheque : public jsoner
{
public:
	/// json objects are split into category, each used as a different "namespace"
    enum category
    {
	filefilter,
	pathfilter,
	command,
	repo,
	confsave,
	conftest,
	confdiff,
	conflist,
	confrest,
	confmerg,
	confrepair,
	confcommon,
	EOE            ///< not a valid value, used for interating in the enum
    };

    bibliotheque() { init(); };
    bibliotheque(const bibliotheque & ref) = default;
    bibliotheque(bibliotheque && ref) noexcept(false) = default;
    bibliotheque & operator = (const bibliotheque & ref) = default;
    bibliotheque & operator = (bibliotheque && ref) noexcept(false) = default;
    virtual ~bibliotheque() = default;

	/// add a new configuration, the name must not exist for that category

	/// \note throw exception_range if name already exist
    void add_config(category categ, const std::string & name, const json & config);

	/// update an existing configuration for that category (name must exist)

	/// \note throw exception_range if name does not exist
    void update_config(category categ, const std::string & name, const json & config);

	/// remove a configuration

	/// \note throw exception_range if name does not exist
    void delete_config(category categ, const std::string & name);

	/// provide a stored configuration

	/// \note throw exception_range if name does not exist
    json fetch_config(category categ, const std::string & name) const;


	/// display beautifulized json data for the given category and name
    std::string display_config(category categ, const std::string & name) const;

	/// list existing configurations under the provided category
    std::deque<std::string> listing(category categ) const;

	/// get saved status

	/// \note saved status is set when adding or modifying config,
	/// but is cleared when loading or saving as json.
    bool get_saved_status() const { return saved; };

	/// clear all stored configurations
    void clear() { init(); };


	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

private:
    typedef std::map<std::string, json> asso;
    typedef std::map<category, asso> table;

    table content;
    mutable bool saved;

    void init();

	/// return true if an entry name in the category cat exist

	/// \param[in] cat category to look into
	/// \param[in] name the configuration name to look for in that category
	/// \param[out] it if found a pointer to the std::pair containing the search cat/name
	/// \param[out] catit if found a pointer to the std::pair of "content" field for that category
	/// \return true if cat+name found. Both it and catit are modified in any case
    bool lookup(category cat,
		const std::string & name,
		asso::iterator & it,
		table::iterator & catit) const;


	/// convert category to json used string
    static std::string category_to_string(category cat);

	/// convert json used string to category
    static category string_to_category(const std::string & s);


	/// current json output version for this class
    static constexpr const unsigned int bibli_version = 1;
    static constexpr const char* category_label = "category";
    static constexpr const char* asso_label = "list";
    static constexpr const char* config_label = "name";
    static constexpr const char* config_def_label = "config";
};

#endif
