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

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <utility>
#include <string>
#include <dar/tools.hpp>

    // webdar headers

    //
#include "bibliotheque.hpp"

using namespace std;

const string bibliotheque::changed = "bibli_changed";

void bibliotheque::add_config(category categ,
			      const string & name,
			      const json & config,
			      const using_set & refs)
{
    table::iterator catit;
    asso::iterator it;
    coordinates coord(categ, name);

    if(lookup(categ, name, it, catit))
	throw exception_range(libdar::tools_printf("A configuration named %s already exists in that category", name.c_str()));

    if(catit == content.end())
	throw WEBDAR_BUG;

    try
    {
	add_dependency_for(coord, refs);
    }
    catch(...)
    {
	remove_dependency_for(coord);
	throw;
    }

    (catit->second)[name] = linked_config(config);
    saved = false;
}

void bibliotheque::update_config(category categ, const
				 string & name,
				 const json & config,
				 const using_set & refs)
{
    table::iterator catit;
    asso::iterator it;
    coordinates coord(categ, name);

    if(! lookup(categ, name, it, catit))
	throw exception_range(libdar::tools_printf("No configuration named %s exists in that category", name.c_str()));

    try
    {
	remove_dependency_for(coord);
	add_dependency_for(coord, refs);
    }
    catch(...)
    {
	remove_dependency_for(coord);
	throw;
    }

    it->second.config = config;
    saved = false;
    act(changed);
}

void bibliotheque::delete_config(category categ, const string & name)
{
    table::iterator catit;
    asso::iterator it;

    if(! lookup(categ, name, it, catit))
	throw exception_range(libdar::tools_printf("No configuration named %s exists in that category", name.c_str()));

    if(! it->second.dependency.empty())
    {
	string errmsg = libdar::tools_printf("Configuration %s/%s cannot be deleted because it is used by:",
					     category_to_string(categ).c_str(),
					     name.c_str());

	for(set<coordinates>::iterator list = it->second.dependency.begin();
	    list != it->second.dependency.end();
	    ++list)
	    errmsg += libdar::tools_printf(" %s/%s",
					   category_to_string(list->cat).c_str(),
					   list->confname);

	throw exception_range(errmsg);
    }

    remove_dependency_for(coordinates(categ, name));
    catit->second.erase(it);
    saved = false;
    act(changed);
}

json bibliotheque::fetch_config(category categ, const string & name) const
{
    table::iterator catit;
    asso::iterator it;

    if(! lookup(categ, name, it, catit))
	throw exception_range(libdar::tools_printf("No configuration named %s exists in that category", name.c_str()));

    return it->second.config;
}

string bibliotheque::display_config(category categ, const string & name) const
{
    return fetch_config(categ, name).dump(4);
}

deque<string> bibliotheque::listing(category categ) const
{
    deque<string> ret;
    table::const_iterator catit = content.find(categ);

    if(catit == content.end())
	throw WEBDAR_BUG;

    for(asso::const_iterator it = catit->second.begin(); it != catit->second.end(); ++it)
	ret.push_back(it->first);

    return ret;
}

void bibliotheque::load_json(const json & source)
{
    unsigned int version;
    string class_id;
    json config = unwrap_config_from_json_header(source,
						 version,
						 class_id);

    string cat_name;
    asso tmp_map;
    string config_name;
    set<coordinates> depend;

    json tmp_json;
    json config_json;
    json used_by;

    content.clear();
    saved = true;

    try
    {
	if(class_id != "bibliotheque")
	    throw exception_range("Unexpected json data: wrong class_id in json header for class bibliotheque");

	if(version > bibli_version)
	    throw exception_range("Version too recent for a bibliotheque json version, upgrade webdar your software");

	if(! config.is_array())
	    throw exception_range("Unexpected json data: bibliotheque configuration is not an array");

	for(json::iterator catit = config.begin(); catit != config.end(); ++catit)
	{

	    if(! catit->is_object())
		throw exception_range("Unexpected json data: bibliotheque per category data is not an object");

	    cat_name = catit->at(category_label);
	    tmp_json = catit->at(asso_label);
	    tmp_map.clear();

	    if(! tmp_json.is_array() && ! tmp_json.is_null())
		throw exception_range("Unexpected json data: bibliotheque list of configuration per category is not an array");

	    for(json::iterator it = tmp_json.begin(); it != tmp_json.end(); ++it)
	    {
		config_name = it->at(config_label);
		config_json = it->at(config_def_label);
		used_by = it->at(config_depend);

		if(! config_json.is_object())
		    throw exception_range("Unexpected json data: component configuration is not an object");

		if(! used_by.is_array() && ! used_by.is_null())
		    throw exception_range("Unexpected json data: component depency list is not a list");

		depend.clear();
		for(json::iterator usit = used_by.begin(); usit != used_by.end(); ++usit)
		    depend.insert(coordinates(it->at(category_label), it->at(config_label)));

		tmp_map[config_name] = linked_config(config_json, depend);
	    }

	    content[string_to_category(cat_name)] = tmp_map;
	}

	    // need to check that all category are present, else add them empty

	string errmsg;

	for(int cat = filefilter; cat != EOE; ++cat)
	    if(content.find(static_cast<category>(cat)) == content.end()) // missing category
	    {
		content[static_cast<category>(cat)] = asso();
		errmsg += " " + category_to_string(static_cast<category>(cat));
	    }

	act(changed);

	if(!errmsg.empty())
	    throw exception_range("The following configuration category were missing and have been reset: " + errmsg);
    }
    catch(json::exception & e)
    {
	throw exception_json("Error reading configuration from json data", e);
    }
}

json bibliotheque::save_json() const
{

    json config;
    json percat_config;
    json tmp;
    json used_by;
    json item;

    for(table::const_iterator catit = content.begin(); catit != content.end(); ++catit)
    {
	percat_config.clear();

	for(asso::const_iterator it = catit->second.begin(); it != catit->second.end(); ++it)
	{
	    tmp.clear();
	    tmp[config_label] = it->first;
	    tmp[config_def_label] = it->second.config;

	    used_by.clear();

	    for(set<coordinates>::const_iterator usit = it->second.dependency.begin();
		usit != it->second.dependency.end();
		++usit)
	    {
		item.clear();
		item[category_label] = category_to_string(usit->cat);
		item[config_label] = usit->confname;
		used_by.push_back(item);
	    }

	    tmp[config_depend] = used_by;

	    percat_config.push_back(tmp);
	}

	tmp.clear();
	tmp[category_label] = category_to_string(catit->first);
	tmp[asso_label] = percat_config;
	config.push_back(tmp);
    }

    saved = true;
    return wrap_config_with_json_header(bibli_version, "bibliotheque", config);
}

void bibliotheque::init()
{
    for(int cat = filefilter; cat != EOE; ++cat)
	content[static_cast<category>(cat)] = asso(); //empty asso

    saved = true;
    register_name(changed);
}

bool bibliotheque::lookup(category cat, const string & name, asso::iterator & it, table::iterator & catit) const
{
    catit = const_cast<table*>(&content)->find(cat);

    if(catit == content.end())
	throw WEBDAR_BUG;

    it = catit->second.find(name);

    return it != catit->second.end();
}

void bibliotheque::add_dependency_for(coordinates user, const using_set & referred)
{
    asso::iterator it;
    table::iterator catit;

    for(set<coordinates>::const_iterator usit = referred.begin();
	usit != referred.end();
	++usit)
    {
	if(! lookup(usit->cat, usit->confname, it, catit))
	    throw exception_range(libdar::tools_printf("config %s/%s relies on non-existant %s/%s configuration",
						       category_to_string(user.cat).c_str(),
						       user.confname.c_str(),
						       category_to_string(usit->cat),
						       usit->confname.c_str()));

	it->second.dependency.insert(user);
    }
}

void bibliotheque::remove_dependency_for(coordinates user)
{
    for(table::iterator catit = content.begin();
	catit != content.end();
	++catit)
    {
	for(asso::iterator it = catit->second.begin();
	    it != catit->second.end();
	    ++it)
	    it->second.dependency.erase(user);
    }
}

string bibliotheque::category_to_string(category cat)
{
    switch(cat)
    {
    case filefilter:
	return "filefilter";
    case pathfilter:
	return "pathfilter";
    case command:
	return "command";
    case repo:
	return "repo";
    case confsave:
	return "confsave";
    case conftest:
	return "conftest";
    case confdiff:
	return "confdiff";
    case conflist:
	return "conflist";
    case confrest:
	return "confrest";
    case confmerg:
	return "confmerg";
    case confrepair:
	return "confrepair";
    case confcommon:
	return "confcommon";
    case EOE:
	throw WEBDAR_BUG;
    default:
	throw WEBDAR_BUG;
    }
}

bibliotheque::category bibliotheque::string_to_category(const string & s)
{
    if(s == "filefilter")
	return filefilter;
    else if(s == "pathfilter")
	return pathfilter;
    else if(s == "command")
	return command;
    else if(s == "repo")
	return repo;
    else if(s == "confsave")
	return confsave;
    else if(s == "conftest")
	return conftest;
    else if(s == "confdiff")
	return confdiff;
    else if(s == "conflist")
	return conflist;
    else if(s == "confrest")
	return confrest;
    else if(s == "confmerg")
	return confmerg;
    else if(s == "confrepair")
	return confrepair;
    else if(s == "confcommon")
	return confcommon;
    else
	throw exception_range(libdar::tools_printf("Unknown bibliotheque category: %s", s.c_str()));
}

