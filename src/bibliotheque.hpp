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
#include "events.hpp"

    /// \file bibliotheque.hpp defines bibliotheque class


    /// class bibliotheque

    /// class bibliotheque purpose is associate json configurations
    /// to a name and a category, and provide means to store, search,
    /// update and delete them as a whole. This class provides its own
    /// configuration (including configuration of it stores) as a jsoner
    /// class.

class bibliotheque : public jsoner, public events
{
public:
    static const std::string changed; /// triggered when the bibliotheque content changes

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

	/// convert category to json used string
    static std::string category_to_string(category cat);

	/// convert json used string to category
    static category string_to_category(const std::string & s);


	/// complete configuration designation
    struct coordinates
    {
	category cat;
	std::string confname;

	coordinates(category c, const std::string & name): cat(c), confname(name) {};
	bool operator < (const coordinates & ref) const { return cat < ref.cat || (cat == ref.cat && confname < ref.confname); };
    };

	/// list of configs a given config depends on
    typedef std::set<coordinates> using_set;

    bibliotheque() { init(); };
    bibliotheque(const bibliotheque & ref) = default;
    bibliotheque(bibliotheque && ref) noexcept(false) = default;
    bibliotheque & operator = (const bibliotheque & ref) = default;
    bibliotheque & operator = (bibliotheque && ref) noexcept(false) = default;
    virtual ~bibliotheque() = default;

	/// add a new configuration, the name must not exist for that category

	/// \note throw exception_range if name already exist
    void add_config(category categ, const std::string & name, const json & config, const using_set & refs = std::set<coordinates>());

	/// update an existing configuration for that category (name must exist)

	/// \note throw exception_range if name does not exist
    void update_config(category categ, const std::string & name, const json & config, const using_set & refs = std::set<coordinates>());

	/// remove a configuration

	/// \note throw exception_range if name does not exist
    void delete_config(category categ, const std::string & name);


	/// tells whether a given cat/config exists
    bool has_config(category categ, const std::string & name) const;

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
    void clear() { init(); act(changed); };


	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override { clear(); };

private:

	/// json configuration and list of other configs that depend on it
    struct linked_config
    {
	json config;
	std::set<coordinates> dependency;

	linked_config() {};
	linked_config(const json & conf) { config = conf; };
	linked_config(const json & conf, const std::set<coordinates> & deps): config(conf), dependency(deps) {};
    };

	/// associate a config name to a json configuration (inside a given category)
    typedef std::map<std::string, linked_config> asso;

	///< associate a category to a list of name+config
    typedef std::map<category, asso> table;

    table content;       ///< all configurations classified per categories
    mutable bool saved;  ///< whether content as changed since last save_json() invocation

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

    void add_dependency_for(coordinates user, const using_set & referred);
    void remove_dependency_for(coordinates user);


	/// current json output version for this class
    static constexpr const unsigned int bibli_version = 1;
    static constexpr const char* category_label = "category";
    static constexpr const char* asso_label = "list";
    static constexpr const char* config_label = "name";
    static constexpr const char* config_def_label = "config";
    static constexpr const char* config_depend = "used-by";
};

#endif
