/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2022 Denis Corbin
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

#ifndef BODY_BUILDER_HPP
#define BODY_BUILDER_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <map>
#include <vector>

    // webdar headers
#include "request.hpp"
#include "css.hpp"
#include "chemin.hpp"
#include "css_library.hpp"

class body_builder : public css
{
public:
	/// constructor
    body_builder() { clear(); };

	/// copy constructor allowed only if the object has no parent nor child
    body_builder(const body_builder & ref);

	/// move constructor
    body_builder(body_builder && ref) noexcept = default;

	/// assignment operator copy constructor allowed only if the object has no parent nor child
    body_builder & operator = (const body_builder & ref);

	/// move operator
    body_builder & operator = (body_builder && ref) noexcept = default;

	/// the (virtual) destructor
    virtual ~body_builder() { orphan_all_children(); unrecord_from_parent(); };




	/// set the root path at which this object will be located in the URL path
	///
	/// \note this is only used if this object has no parent
    void set_prefix(const chemin & prefix);


	/// Common interface for class that have to ask other body_builder to provide their own body_part()
	///
	/// this call is not mandatory, but let a body_builder the possibility to rely on a set of
	/// body_builder protected routines that will build the body part recursively from the children
	/// adopted this way.
	/// \note this call semantic is that the adopted object becomes a child of the body_builder.
	/// The adopted child stays managed by its real parent, the adoption relationship builds a tree
	/// of dependant objects using one another to provide a body_part (see get_body_part() method).
	/// this tree is automatically broken when an object is destroyed in the way that all recorded
	/// children exists (the recorded addresses always points to real existing objects)
    void adopt(body_builder *obj);

	/// Common interface for class that have to ask other body_builder to provide their own body_part()
	///
	/// this call is not mandatory, it is the opposite action of adopt(). After this call
	/// the given object is no more sollicited to build a body part.
	/// \note if the requested object is not known an exception is thrown
    void foresake(body_builder *obj);


	/// ask for the object to become visible in HTML page or temporarily hidden
    void set_visible(bool mode) { next_visible = mode; };

	/// returns the current visible status of the object
	///
	/// \note the returned visible status becomes what is set calling set_visible() method
	/// only once the object has acknoledged its new status thanks to the ack_visible() method.
	/// The object can known the current visible status but also the next_visible() status, which
	/// is the one that is pending for acknoledgment.
    bool get_visible() const { return visible; };

	/// what the future but still non acknoledged visible status
    bool get_next_visible() const { return next_visible; };

    	/// replace css properties by a class of given name

	/// \param[in] classname, is the name of the css class to use
	/// \param[in,out] the class will have all css parameters of ref and ref css properties
	/// will be replaced by a css reference to the given class
    void move_css_properties_to_html_class(const std::string & classname);

	/// ask the object to provide a part of the body to answer the request
	///
	/// \param[in] path is the full path, but the path.index points to the asked object name
	/// \param[in] req is the request to be answered, the uri's path part targets the
	///   choice of child to use for the sub-response
	/// \return the body part to be send in the response (html code or other depending
	///   on the request)
	/// \note if path is an empty list only this object "body_builder" object can be
	/// used to answer, no child object can be invoked by mean of its name.
	/// \note when going down to the leaf of the tree, the path get shorter by removing
	/// the first items one at each step. Empty path means the object itslef
	/// \note the inherited class should take the visible status in consideration when
	/// returning HTML from this call
    virtual std::string get_body_part(const chemin & path,
				      const request & req) = 0;

	/// ask for the implementation not to add a new line after this control
    void set_no_CR() { no_CR = true; };

protected:

	/// inherited from class css
	/// virtual void css_updated(bool inherited) override;

	/// return the path of 'this' according to its descent in the body_builder tree of adopted children
    chemin get_path() const;

	/// returns the name of 'this' if it has been adopted by a parent body_builder object
	///
	/// \note, an empty string is returned if the object has not been adopted
    std::string get_recorded_name() const;


	/// let objects of inherited class storing a css_library that will be accessible from its children

	/// \note a child object, directly or indirectly (grand child, aso) will be
	/// able to access and populate this css_library using the lookup_css_library() method
    void store_css_library();

	/// lookup toward registered parent body_builder object for the closest stored css_library

	/// \note if neither the present object nor any of its parent stores an css_library,
	/// the call unique_ptr is false (points to nullptr), else it points to the found css_library
    std::unique_ptr<css_library> & lookup_css_library();

	/// return the class specification to be inserted inline beside css style

	/// \note this call also validate that the html_class exist in the css_library
    std::string get_html_classes() const;

	/// let a parent obtain the body part from one of its children given its official name and seen the path of the request
	///
	/// \param[in] path, this is the path exactly as received from the get_body_part call:
	/// the first member is the name of a child object.
	/// \param[in] req, this is the request exactly as received from the get_body_part call
	/// \note it is typically called from the parent get_body_part() method
	/// \note if path is an empty list, the call fails (throwing an exception) as no
	/// no further name is available to find an child object to return its body part.
	/// In consequence, the caller must check whether the path is empty() or not before
	/// invoking this method
	/// \note if the object is not visible, this call will return an empty string
    std::string get_body_part_from_target_child(const chemin & path,
						const request & req);

	/// let a parent obtain the body part from all children in the order the have been adopted
	///
	/// \param[in] path, this is the path, it can be empty. If not the front member is poped from the target
	/// even if the poped part of the path does not match the name of the consulted child object
	/// \param[in] req, this is the request exactly as received from the get_body_part call
	/// \note if the object is not visible, this call will return an empty string
    std::string get_body_part_from_all_children(const chemin & path,
						const request & req);

	/// For inherited class, called when the path has changed, tipically when this object has been adopted or foresaken, or a parent did
    virtual void path_has_changed() {};

	/// Be informed that a new child has been adopted
    virtual void has_been_adopted(body_builder *obj) {};

	/// Be informed that a child has been foresaken
    virtual void has_been_foresaken(body_builder *obj) {};

	/// Be informed we have been adopted by obj the parent
    virtual void has_been_adopted_by(body_builder *obj) {};

	/// Be informed we have been foresaken by obj, our former parent
    virtual void has_been_foresaken_by(body_builder *obj) {};

	/// access to adopted childs
    unsigned int size() const { return order.size(); };

    body_builder *operator[] (unsigned int i) { return order[i]; };

	/// orphan all adopted children
    void orphan_all_children();

	/// true if it has been requested no to add Carriage Return after the HTML object
    bool get_no_CR() const { return no_CR; };

	/// acknoledge the new visible status
    void ack_visible() { visible = next_visible; };


private:
    bool visible;
    bool next_visible;
    chemin x_prefix;
    bool no_CR;
    body_builder *parent;
    std::vector<body_builder *> order;
    std::map<std::string, body_builder *> children;
    std::map<body_builder *, std::string> revert_child;
    std::unique_ptr<css_library> library;

	/// unrecord 'this' from its parent as a adopted child
    void unrecord_from_parent();

	/// inform children and all their descendant children that the path has changed calling their path_has_changed() method
    void recursive_path_has_changed();

	/// (re)initialize fields to default sane values
    void clear() { parent = nullptr; order.clear(); children.clear(); revert_child.clear(); visible = true ; next_visible = true; no_CR = false; };
};

#endif
