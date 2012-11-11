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




chemin body_builder::get_path() const
{
    chemin ret;
    std::map<body_builder *, string>::const_iterator it;

    if(parent != NULL)
    {
	it = parent->revert_child.find(const_cast<body_builder *>(this));
	if(it == parent->revert_child.end())
	    throw WEBDAR_BUG;
	ret = parent->get_path();
	ret.push_back(it->second); // adding the name of "this" to parent's path
    }
    else // no parent, we are the root, so the path is empty
	ret.clear();

    return ret;
}


string body_builder::get_recorded_name() const
{
    if(parent != NULL)
    {
	map<body_builder *, string>::const_iterator it = parent->revert_child.find(const_cast<body_builder *>(this));
	if(it == parent->revert_child.end())
	    throw WEBDAR_BUG;
	return it->second;
    }
    else
	return "";
}

string body_builder::record_child(body_builder *obj)
{
    string new_name;
    map<string, body_builder *>::iterator it;
    map<body_builder *, string>::iterator rit = revert_child.find(obj);

    if(rit != revert_child.end())
	throw WEBDAR_BUG; // object already recorded

    if(obj->parent != NULL)
	throw WEBDAR_BUG; // object already recorded in another parent

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

    return new_name;
}

void body_builder::unrecord_child(body_builder *obj)
{
    map<body_builder *, string>::iterator rit = revert_child.find(obj);
    list<body_builder *>::iterator ot = find(order.begin(), order.end(), obj);

    if(ot == order.end())
	throw WEBDAR_BUG;

    if(rit != revert_child.end())
    {
	string name = rit->second;
	map<string, body_builder *>::iterator it = children.find(name);
	if(it == children.end())
	    throw WEBDAR_BUG;
	children.erase(it);
	revert_child.erase(rit);
	obj->parent = NULL;
	order.erase(ot);
    }
    else
	throw WEBDAR_BUG; // unknown object asked for removal
}


string body_builder::get_body_part_from_target_child(const chemin & path,
						     const request & req)
{
    if(path.empty())
	throw WEBDAR_BUG; // invoked with an empty path

    string name = path.front();
    map<string, body_builder *>::iterator it = children.find(name);

    if(it != children.end())
    {
	chemin sub_path = path;
	sub_path.pop_front();
	return it->second->get_body_part(sub_path, req);
    }
    else
	throw exception_input("unkown URL requested", STATUS_CODE_NOT_FOUND);
}

string body_builder::get_body_part_from_all_children(const chemin & path,
						     const request & req)
{
    string ret = "";
    chemin sub_path = path;
    list<body_builder *>::iterator it = order.begin();

    if(!sub_path.empty())
	sub_path.pop_front();

    while(it != order.end())
    {
	if(*it == NULL)
	    throw WEBDAR_BUG;
	ret += (*it)->get_body_part(sub_path, req);
	++it;
    }

    return ret;
}

void body_builder::clear_children()
{
    map<string, body_builder *>::iterator it = children.begin();
    while(it != children.end())
    {
	unrecord_child(it->second);
	it = children.begin();
	    // unrecord_child modifies the "children" map
	    // so we set 'it' to a valid value setting it to begin()
	    // but at each round, a item is removed from the children map
	    // so ghd loop will end when the map will become empty
    }

    children.clear();
    revert_child.clear();
}

void body_builder::unrecord_from_parent()
{
    if(parent != NULL)
	parent->unrecord_child(this);
}
