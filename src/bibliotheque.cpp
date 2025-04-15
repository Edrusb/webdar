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

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <utility>
#include <string>
#include <deque>
#include <dar/tools.hpp>

    // webdar headers

    //
#include "bibliotheque.hpp"

using namespace std;

string bibliotheque::changed(category cat)
{
    string ret(string(myclass_id) + "::");

    switch(cat)
    {
    case filefilter:
	ret += "filefilter";
	break;
    case pathfilter:
	ret += "pathfilter";
	break;
    case command:
	ret += "command";
	break;
    case repo:
	ret += "repo";
	break;
    case compress:
	ret += "compr";
	break;
    case confsave:
	ret += "save";
	break;
    case conftest:
	ret += "test";
	break;
    case confdiff:
	ret += "diff";
	break;
    case conflist:
	ret += "list";
	break;
    case confrest:
	ret += "restore";
	break;
    case confmerge:
	ret += "merge";
	break;
    case confrepair:
	ret += "repaor";
	break;
    case confcommon:
	ret += "common";
	break;
    case slicing:
	ret += "slicing";
	break;
    case ciphering:
	ret += "ciphering";
	break;
    case delta_sig:
	ret += "delta-sig";
	break;
    case over_policy:
	ret += "over-policy";
	break;
    case confread:
	ret += "read";
	break;
    case confisolate:
	ret += "isolate";
	break;
    case EOE:
	throw WEBDAR_BUG;
    default:
	throw WEBDAR_BUG;
    }

    return ret;
}

bibliotheque::category & operator++(bibliotheque::category & cat)
{
    if(cat < bibliotheque::filefilter)
	throw WEBDAR_BUG;

    if(cat >= bibliotheque::EOE)
	throw WEBDAR_BUG;

    cat = static_cast<bibliotheque::category>(cat + 1);
    return cat;
}

bibliotheque::bibliotheque()
{
    category cat = filefilter;
    init();

    while(cat != EOE)
    {
	register_name(changed(cat));
	++cat;
    }
}

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
	using_set empty;

	add_dependency_for(coord, refs);
	check_against_cyclic_dependencies(coord, empty);
    }
    catch(...)
    {
	remove_dependency_for(coord);
	throw;
    }

    (catit->second)[name] = linked_config(config);
    saved = false;
    act(changed(categ));
}


void bibliotheque::add_external_ref_to(category categ, const string & name, const void* from_where)
{
    asso::iterator it;
    table::iterator catit;
    coordinates coord(categ, name);

    map<coordinates, refs>::iterator ut = outside.find(coord);

    if(!lookup(categ,
	       name,
	       it,
	       catit))
    {
	    // problem, the configuration we refer to does not exist!

	if(ut != outside.end())
	    throw WEBDAR_BUG; // but an external ref to it already exist!
	else
	    throw WEBDAR_BUG; // config unknown from external refs datastructure
    }

    if(ut != outside.end())
    {
	if(ut->second.find(from_where) != ut->second.end())
	    throw WEBDAR_BUG; // already referred by the same object/place/ref

	ut->second.insert(from_where);
    }
    else
    {
	refs tmp;
	tmp.insert(from_where);

	outside[coord] = tmp;
    }
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
	using_set empty;

	remove_dependency_for(coord);
	add_dependency_for(coord, refs);
	check_against_cyclic_dependencies(coord, empty);
    }
    catch(...)
    {
	remove_dependency_for(coord);
	throw;
    }

    it->second.config = config;
    saved = false;
    act(changed(categ));
}

void bibliotheque::delete_config(category categ, const string & name)
{
    table::iterator catit;
    asso::iterator it;
    map<coordinates, refs>::iterator outit = outside.find(coordinates(categ, name));

    if(! lookup(categ, name, it, catit))
	throw exception_range(libdar::tools_printf("No configuration named %s exists in that category", name.c_str()));

    if(! it->second.dependency.empty())
    {
	string errmsg = libdar::tools_printf("%s configuration named \"%s\" cannot be deleted because it is used by:",
					     category_description(categ).c_str(),
					     name.c_str());

	for(set<coordinates>::iterator list = it->second.dependency.begin();
	    list != it->second.dependency.end();
	    ++list)
	{
	    if(list != it->second.dependency.begin())
		errmsg += ", ";
	    errmsg += libdar::tools_printf(" %s named \"%s\"",
					   category_description(list->cat).c_str(),
					   list->confname.c_str());
	}

	throw exception_range(errmsg);
    }

    if(outit != outside.end()) // having external references
    {
	throw exception_range(
	    libdar::tools_printf("This configuration cannot be deleted, it is used %d time(s) in the user interface (but not used by other configurations)",
				 outit->second.size())
	    );

    }

    remove_dependency_for(coordinates(categ, name));
    catit->second.erase(it);
    saved = false;
    act(changed(categ));
}

void bibliotheque::delete_external_ref_to(category categ, const string & name, const void* from_where)
{
    coordinates coord(categ, name);

    map<coordinates, refs>::iterator ut = outside.find(coord);

    if(ut == outside.end())
	throw WEBDAR_BUG; // no ref to this config!!!

    refs::iterator refit = ut->second.find(from_where);

    if(refit == ut->second.end())
	throw WEBDAR_BUG; // no ref from from_where for this config!!!

    ut->second.erase(refit); // removing the ref

    if(ut->second.empty())  // no more ref to this config
       outside.erase(ut);   // removing entry for that config from outside
}

bool bibliotheque::has_config(category categ, const string & name) const
{
    table::iterator catit;
    asso::iterator it;

    return lookup(categ, name, it, catit);
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

void bibliotheque::clear()
{
    init();
    for(category cat = filefilter;
	cat != EOE;
	++cat)
	act(changed(cat));
}

bool bibliotheque::is_empty() const
{
    bool found = false;
    category cat = filefilter;
    map<category, asso>::const_iterator percat;

    while(cat != EOE && ! found)
    {
	percat = content.find(cat);
	if(percat == content.end())
	    throw WEBDAR_BUG;

	if(!percat->second.empty())
	    found = true;
	++cat;
    }

    return !found;
}

void bibliotheque::load_json(const json & source)
{
    unsigned int version;
    string class_id;
    json config = unwrap_config_from_json_header(source,
						 version,
						 class_id);

    string cat_name;
    category cat;
    set<category> events_to_fire;
    asso tmp_map;
    string config_name;
    set<coordinates> depend;

    json tmp_json;
    json config_json;
    json used_by;
    json categories;
    json globalprop;

    content.clear();
    saved = true;

    try
    {
	if(class_id != myclass_id)
	    throw exception_range(libdar::tools_printf("Unexpected json data: wrong class_id in json header for class %s", myclass_id));

	if(version > format_version)
	    throw exception_range(libdar::tools_printf("Version too recent for a %s json version, upgrade webdar your software", myclass_id));

	globalprop = config.at(jlabel_globalprop);
	categories = config.at(jlabel_categprop);

	autosave = globalprop.at(jlabel_autosave);

	if(! categories.is_array())
	    throw exception_range(libdar::tools_printf("Unexpected json data: %s configuration is not an array", myclass_id));

	for(json::iterator catit = categories.begin(); catit != categories.end(); ++catit)
	{

	    if(! catit->is_object())
		throw exception_range(libdar::tools_printf("Unexpected json data: %s per category data is not an object", myclass_id));

	    cat_name = catit->at(category_label);
	    tmp_json = catit->at(asso_label);
	    tmp_map.clear();

	    if(! tmp_json.is_array() && ! tmp_json.is_null())
		throw exception_range(libdar::tools_printf("Unexpected json data: %s list of configuration per category is not an array", myclass_id));

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
		    depend.insert(coordinates(string_to_category(usit->at(category_label)), usit->at(config_label)));

		tmp_map[config_name] = linked_config(config_json, depend);
	    }

	    cat = string_to_category(cat_name);
	    content[cat] = tmp_map;
	    if(tmp_json.begin() != tmp_json.end()) // non empty definition for that category
		events_to_fire.insert(cat);
	}

	    // need to check that all category are present, else add them empty

	string errmsg;

	for(int cat = filefilter; cat != EOE; ++cat)
	    if(content.find(static_cast<category>(cat)) == content.end()) // missing category
	    {
		content[static_cast<category>(cat)] = asso();
		errmsg += " " + category_to_string(static_cast<category>(cat));
	    }

	for(set<category>::iterator fireit = events_to_fire.begin();
	    fireit != events_to_fire.end();
	    ++fireit)
	    act(changed(*fireit));

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
    json allcategs;
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
	allcategs.push_back(tmp);
    }

    config[jlabel_categprop] = allcategs;

    tmp.clear();
    tmp[jlabel_autosave] = autosave;
    config[jlabel_globalprop] = tmp;

    saved = true;
    return wrap_config_with_json_header(format_version, myclass_id, config);
}

void bibliotheque::init()
{
    for(int cat = filefilter; cat != EOE; ++cat)
	content[static_cast<category>(cat)] = asso(); //empty asso
    saved = true;
    autosave = true;
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
						       category_to_string(usit->cat).c_str(),
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

bibliotheque::using_set bibliotheque::get_direct_dependencies_of(coordinates user) const
{
    using_set ret;

    for(table::const_iterator catit = content.begin();
	catit != content.end();
	++catit)
    {
	for(asso::const_iterator it = catit->second.begin();
	    it != catit->second.end();
	    ++it)
	    if(it->second.dependency.find(user) != it->second.dependency.end())
		ret.insert(coordinates(catit->first, it->first));
    }

    return ret;
}

void bibliotheque::check_against_cyclic_dependencies(coordinates source, const set<coordinates> & seen)
{
    using_set next = seen;
    using_set children = get_direct_dependencies_of(source);

    if(seen.find(source) != seen.end())
	throw exception_range(libdar::tools_printf("Cyclic dependency found, %s/%s depends on itself",
						   category_to_string(source.cat).c_str(),
						   source.confname.c_str()));

    next.insert(source);

    for(using_set::iterator it = children.begin();
	it != children.end();
	++it)
	check_against_cyclic_dependencies(*it, next); // recursion
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
    case compress:
	return "compress";
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
    case confmerge:
	return "confmerge";
    case confrepair:
	return "confrepair";
    case confcommon:
	return "confcommon";
    case slicing:
	return "slicing";
    case ciphering:
	return "ciphering";
    case delta_sig:
	return "delta-sig";
    case over_policy:
	return "over-policy";
    case confread:
	return "confread";
    case confisolate:
	return "confisolate";
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
    else if(s == "compress")
	return compress;
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
    else if(s == "confmerge")
	return confmerge;
    else if(s == "confrepair")
	return confrepair;
    else if(s == "confcommon")
	return confcommon;
    else if(s == "slicing")
	return slicing;
    else if(s == "ciphering")
	return ciphering;
    else if(s == "delta-sig")
	return delta_sig;
    else if(s == "over-policy")
	return over_policy;
    else if(s == "confread")
	return confread;
    else if(s == "confisolate")
	return confisolate;
    else
	throw exception_range(libdar::tools_printf("Unknown category: %s", s.c_str()));
}

string bibliotheque::category_description(category cat)
{
    switch(cat)
    {
    case filefilter:
	return "file filter";
    case pathfilter:
	return "path filter";
    case command:
	return "shell command";
    case repo:
	return "repository";
    case compress:
	return "compression";
    case confsave:
	return "create options";
    case conftest:
	return "test options";
    case confdiff:
	return "compare options";
    case conflist:
	return "list options";
    case confrest:
	return "restore options";
    case confmerge:
	return "merge options";
    case confrepair:
	return "repair options";
    case confcommon:
	return "common options";
    case slicing:
	return "slice options";
    case ciphering:
	return "cipher options";
    case delta_sig:
	return "delta signatures options";
    case over_policy:
	return "overwriting policy";
    case confread:
	return "read options";
    case confisolate:
	return "isolate options";
    case EOE:
	throw WEBDAR_BUG;
    default:
	throw WEBDAR_BUG;
    }
}
