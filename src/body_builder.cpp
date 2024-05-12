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
extern "C"
{

}

    // C++ system header files
#include <algorithm>

    // webdar headers
#include "webdar_tools.hpp"


    //
#include "body_builder.hpp"

using namespace std;

const unsigned int NAME_WIDTH = 4;

body_builder::body_builder(const body_builder & ref)
{
    clear();
    if(ref.parent != nullptr || !ref.order.empty() || !ref.children.empty() || !ref.revert_child.empty())
        throw WEBDAR_BUG;
    visible = ref.visible;
	// x_prefix cannot be the same as ref so it stays to the default value
    no_CR = ref.no_CR;
	// parent cannot be the same as ref so it stays nullptr
	// order, children and revert_child stay defaulted as we do not copy the children
	// last_body_path, past_body_req and last_body_part are kept defaulted also
    library_asked = ref.library_asked;
    if(ref.library)
    {
        library.reset(new (nothrow) css_library(*(ref.library)));
        if(!library)
            throw exception_memory();
    }
    else
        library.reset();
    css_class_names = ref.css_class_names;
}

body_builder & body_builder::operator = (const body_builder & ref)
{
    if(parent != nullptr || !order.empty() || !children.empty() || !revert_child.empty() || library)
        throw WEBDAR_BUG;
    if(ref.parent != nullptr || !ref.order.empty() || !ref.children.empty() || !ref.revert_child.empty() || ref.library)
        throw WEBDAR_BUG;

    visible = ref.visible;
    no_CR = ref.no_CR;
    css_class_names = ref.css_class_names;
    library_asked = ref.library_asked;
    if(ref.library)
    {
        library.reset(new (nothrow) css_library(*(ref.library)));
        if(!library)
            throw exception_memory();
    }
    else
        library.reset(); // drop possibly existing css_library
    last_body_path = ref.last_body_path;
    last_body_req_uri = ref.last_body_req_uri;
    last_body_req_body = ref.last_body_req_body;
    body_changed = ref.body_changed;
    ignore_children_body_changed = ref.ignore_children_body_changed;

    return *this;
}

void body_builder::set_prefix(const chemin & prefix)
{
    if(parent != nullptr)
        throw WEBDAR_BUG;
    x_prefix = prefix;
    recursive_path_has_changed();
    my_body_part_has_changed(); // force reevaluation
}

void body_builder::adopt(body_builder *obj)
{
	// sanity checks

    if(obj == nullptr)
        throw WEBDAR_BUG;

    if(obj == this)
	throw WEBDAR_BUG; // trying to adopt ourself!!!

    const css_library* ancient_css_library = obj->lookup_css_library().get();
    const css_library* new_css_library = lookup_css_library().get();
    string new_name;
    map<string, body_builder*>::iterator it;
    map<body_builder*, string>::iterator rit = revert_child.find(obj);

    if(rit != revert_child.end())
        throw WEBDAR_BUG; // object already recorded / adopted

    if(obj->parent != nullptr)
        throw WEBDAR_BUG; // object already recorded by another parent

	// updating internal data structure to record the adoption

    do
    {
        new_name = webdar_tools_generate_random_string(NAME_WIDTH);
        it = children.find(new_name);
    }
    while(it != children.end());

    order.push_back(obj);
    children[new_name] = obj;
    revert_child[obj] = new_name;
    obj->parent = this;

	// now informing parent and children

    obj->recursive_path_has_changed();
    has_adopted(obj);
    obj->has_been_adopted_by(this);

        // if a css_library is available to the child thanks to the adoption
        // we trigger all the child lineage to record its css_classes
    if(new_css_library != ancient_css_library)
        obj->recursive_new_css_library_available();

    my_body_part_has_changed();
}

void body_builder::foresake(body_builder *obj)
{
	// sanity checks

    if(obj == nullptr)
        throw WEBDAR_BUG;

    map<body_builder *, string>::iterator rit = revert_child.find(obj);
    vector<body_builder *>::iterator ot = find(order.begin(), order.end(), obj);

    if(ot == order.end()) // object not found in the ordered list
        throw WEBDAR_BUG;

	// informing parent and children that we are about to foresake

    obj->will_be_foresaken_by(this);
    will_foresake(obj);

	// updating internal data structure about the leave

    if(rit != revert_child.end())
    {
        string name = rit->second;
        map<string, body_builder *>::iterator it = children.find(name);
        if(it == children.end()) // object known by both ordered list and rever_child map, but unknown by children map
            throw WEBDAR_BUG;

            // removing the object from the three lists/maps
        children.erase(it);
        revert_child.erase(rit);
        order.erase(ot);

            // unrecording us as its parent
        if(obj->parent == nullptr)
            throw WEBDAR_BUG;
        obj->parent = nullptr;
    }
    else
        throw WEBDAR_BUG; // object known in the ordered list but unknown by the revert_child map

    obj->recursive_path_has_changed();
    my_body_part_has_changed();
}

void body_builder::set_visible(bool mode)
{
    if(visible != mode)
    {
	visible = mode;
	my_body_part_has_changed();  // keep trace of the change at body_builder level
	my_visibility_has_changed(); // inform inherited class that overwrote this method
    }
}

void body_builder::add_css_class(const string & name)
{
    if(css_class_names.find(name) != css_class_names.end())
        throw exception_range(string("the css_class name to add is already present: ") + name);

    css_class_names.insert(name);
    css_classes_have_changed();
    my_body_part_has_changed();
}

void body_builder::add_css_class(const css_class_group & cg)
{
    string name;

    cg.reset_read();
    while(cg.read_next(name))
        add_css_class(name);
    css_classes_have_changed();
    my_body_part_has_changed();
}

bool body_builder::has_css_class(const string & name) const
{
    return css_class_names.find(name) != css_class_names.end();
}

void body_builder::remove_css_class(const string & name)
{
    if(css_class_names.find(name) == css_class_names.end())
        throw exception_range(string("the css_class name to remove is not present in the list: ") + name);

    css_class_names.erase(name);
    css_classes_have_changed();
    my_body_part_has_changed();
}

void body_builder::remove_css_class(const css_class_group & cg)
{
    string name;

    cg.reset_read();
    while(cg.read_next(name))
        remove_css_class(name);
    css_classes_have_changed();
    my_body_part_has_changed();
}

css_class_group body_builder::get_css_class_group() const
{
    css_class_group ret;
    set<string>::iterator it = css_class_names.begin();

    while(it != css_class_names.end())
    {
        ret.add_css_class(*it);
        ++it;
    }

    return ret;
}

string body_builder::get_css_classes() const
{
    string ret;
    set<string>::const_iterator it = css_class_names.begin();

    while(it != css_class_names.end())
    {
        if(!ret.empty())
            ret += " ";
        ret += *it;

        ++it;
    }

    if(!ret.empty())
        ret = "class=\"" + ret + "\"";

    return ret;
}

void body_builder::define_css_class_in_library(const css_class & csscl)
{
    unique_ptr<css_library> & csslib = lookup_css_library(); // this is a reference to a unique_ptr

    if(!csslib)
        throw WEBDAR_BUG;

    csslib->add(csscl);
}

void body_builder::define_css_class_in_library(const string & name, const css & cssdef)
{
    define_css_class_in_library(css_class(name, cssdef));
}


bool body_builder::is_css_class_defined_in_library(const string & name) const
{
    unique_ptr<css_library> & csslib = lookup_css_library(); // this is a reference to a unique_ptr

    if(!csslib)
        return false;
    else
        return csslib->class_exists(name);
}

string body_builder::get_body_part(const chemin & path,
				   const request & req)
{
    static const unsigned int maxloop = 100;
    string ret;

    create_css_lib_if_needed();

    if(parent != nullptr)
    {
	ret = get_body_part_or_cache(path, req);
    }
    else // root of the adoption tree
    {
	request localreq = req;
	unsigned int loop = 0;

	flush_all_cached_body_part();
	    // now all children will generate events
	    // and so on accordingly to the received
	    // request:

	do
	{
	    ret = get_body_part_or_cache(path, localreq);

	    if(body_changed) // one of my child or myself has changed
		localreq.post_to_get();
		    // we avoid regenerating post events

	    if(++loop >= maxloop)
		throw WEBDAR_BUG;
	}
	while(body_changed);
    }

    return ret;
}

void body_builder::my_body_part_has_changed()
{
    if(ignore_children_body_changed)
	return; // we just ignore this call

    body_changed = true;
    if(parent != nullptr)
	parent->my_body_part_has_changed();
}

chemin body_builder::get_path() const
{
    chemin ret;
    map<body_builder *, string>::const_iterator it;

    if(parent != nullptr)
    {
        it = parent->revert_child.find(const_cast<body_builder *>(this));
        if(it == parent->revert_child.end())
            throw WEBDAR_BUG;
        ret = parent->get_path();
        ret.push_back(it->second); // adding the name of "this" to parent's path
    }
    else // no parent, we are the root, so the path is given by prefix
        ret = x_prefix;

    return ret;
}


string body_builder::get_recorded_name() const
{
    if(parent != nullptr)
    {
        map<body_builder *, string>::const_iterator it = parent->revert_child.find(const_cast<body_builder *>(this));
        if(it == parent->revert_child.end())
            throw WEBDAR_BUG;
        return it->second;
    }
    else
        return "";
}

unique_ptr<css_library> & body_builder::lookup_css_library() const
{
    const_cast<body_builder*>(this)->create_css_lib_if_needed();
    if(library || parent == nullptr)
        return const_cast<unique_ptr<css_library>&>(library);
    else return parent->lookup_css_library();
}


string body_builder::get_body_part_from_target_child(const chemin & path,
                                                     const request & req)
{
    string ret;

    create_css_lib_if_needed();

    if(path.empty())
        throw WEBDAR_BUG; // invoked with an empty path

    string name = path.front();
    map<string, body_builder *>::iterator it = children.find(name);

    if(it != children.end())
    {
	chemin sub_path = path;
	sub_path.pop_front();
	ret = it->second->get_body_part(sub_path, req);
    }
    else
	throw exception_input("unkown URL requested", STATUS_CODE_NOT_FOUND);

    if(visible)
	return ret;
    else
	return "";
}

string body_builder::get_body_part_from_all_children(const chemin & path,
                                                     const request & req)
{
    string ret = "";
    chemin sub_path = path;
    vector<body_builder *>::iterator it = order.begin();

    create_css_lib_if_needed();

    if(!sub_path.empty())
	sub_path.pop_front();

    while(it != order.end())
    {
	if(*it == nullptr)
	    throw WEBDAR_BUG;
	ret += (*it)->get_body_part(sub_path, req);
	++it;
    }

    if(visible)
	return ret;
    else
	return "";
}

void body_builder::orphan_all_children()
{
    body_builder *obj = nullptr;

    map<string, body_builder *>::iterator it = children.begin();
    while(it != children.end())
    {
        obj = it->second;
        if(obj == nullptr)
            throw WEBDAR_BUG;
        try
        {
            foresake(obj);
        }
        catch(...)
        {
            throw WEBDAR_BUG;
        }

        it = children.begin();
            // unrecord_child modifies the "children" map
            // so we set 'it' to a valid value setting it to begin()
            // but at each round, a item is removed from the children map
            // so the loop will end when the map will become empty
    }

    if(children.size() != 0)
        throw WEBDAR_BUG;
    if(order.size() != 0)
        throw WEBDAR_BUG;
    if(revert_child.size() != 0)
        throw WEBDAR_BUG;
}


void body_builder::unrecord_from_parent()
{
    if(parent != nullptr)
        parent->foresake(this);
}

void body_builder::recursive_path_has_changed()
{
    vector<body_builder *>::iterator it = order.begin();

    path_has_changed();
    while(it != order.end())
    {
        if((*it) == nullptr)
            throw WEBDAR_BUG;
        (*it)->recursive_path_has_changed();
        ++it;
    }
}

void body_builder::recursive_new_css_library_available()
{
    vector<body_builder *>::iterator it = order.begin();

    new_css_library_available();
    while(it != order.end())
    {
        if((*it) == nullptr)
            throw WEBDAR_BUG;
        (*it)->recursive_new_css_library_available();
        ++it;
    }
}

void body_builder::clear()
{
    visible = true;
    no_CR = false;
    parent = nullptr;
    order.clear();
    children.clear();
    revert_child.clear();
    last_body_path.clear();
    last_body_req_uri.clear();
    last_body_req_body.clear();
    last_body_part.clear();
    library_asked = false;
    library.reset();
    css_class_names.clear();
    body_changed = true;
    ignore_children_body_changed = false;
}

void body_builder::create_css_lib_if_needed()
{
    if(library_asked)
    {
	if(library)
	    throw exception_range("A css_library is already stored for that body_builder object");

	library.reset(new (nothrow) css_library());

	if(!library)
	    throw exception_memory();
	else
	    library_asked = false;

	recursive_new_css_library_available();
    }
}

void body_builder::flush_all_cached_body_part()
{
    vector<body_builder*>::iterator it = order.begin();

    body_changed = true;
    while(it != order.end())
    {
	if((*it) == nullptr)
	    throw WEBDAR_BUG;
	(*it)->flush_all_cached_body_part();
	++it;
    }
}

string body_builder::get_body_part_or_cache(const chemin & path,
					    const request & req)
{
    string ret;

    if(path == last_body_path
       && req.get_uri() == (const uri)(last_body_req_uri)
       && req.get_body() == last_body_req_body
       && ! body_changed)
    {
	ret = last_body_part;
    }
    else
    {
	body_changed = false; // set before to track any changes implied by the following line
	if(visible)
	{
	    ret = inherited_get_body_part(path, req);
	    if(! body_changed)
	    {
		last_body_path = path;
		last_body_req_uri = req.get_uri();
		last_body_req_body = req.get_body();
		last_body_part = ret;
	    }
		// else, if body_changed is already true
		// whatever we would store in last_* fields
		// would not be used
	}
	else
	    ret = "";
	    // we don't record the current status
	    // as the object is not visible, and
	    // when it will become visible, set_visible()
	    // will set body_changed to true
	    // calling my_body_part_has_changed()
	    // so whatever we would have stored in last_*
	    // field would not be used anyway.
    }

    return ret;
}
