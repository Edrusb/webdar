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
#include "chemin.hpp"
#include "css_library.hpp"
#include "css_class_group.hpp"


    /// class body_builder is the root class of object generating HTML body

    /// - the inherited_get_body_part() method let inherited class define what is the HTML
    ///   body to be generated based on the request passed in argument. Also passed
    ///   in argument is the path, which is comes from the URI of the request.
    ///   From the outside, this is the get_body_part() that is to be used it implements
    ///   a caching layout over inherited_get_body_part(), manage visibility() attribute.
    ///   About the caching layout, an inherited class can flush cache and force a new
    ///   evaluation of its generated body by calling my_body_part_has_changed().
    /// - A body_builder object is expected to be located at given path of the requested URL
    ///   and this let define its behavior according to the path of the request and its own
    ///   place in the path tree. This place is defined by the adoption() mechanism:
    /// - body_builder objects can adopt other body_builder object creating a tree
    ///   topology of body_builder. The path of an object adopted by a parent object
    ///   is a subdirectory of the parent's path. It is a unique name randomly chosen
    ///   by the parent at adoption time. See get_path(), get_recorded_name()
    /// - the visibility property of an body_builder let it keep its place in the
    ///   path and adoption tree without returning any HTML code temporarily, this is
    ///   set using set_visible() method and managed inside get_body_part()
    /// - the get_body_part() of a parent can thus rely on the get_body_part() of its
    ///   adopted children, this is the freedom of the parent class do decide how to
    ///   compose or ignore its childen and the possible HTML code they can return, but several
    ///   method are provided for common tasks: get_body_part_from_target_child() as well
    ///   as get_body_part_from_all_children().
    /// - Several protected hooks and methods are provided for inherited class to be
    ///   informed of their adoption or foresake: has_adopted() has_been_adopted_by(),
    ///   will_foresake(), will_be_foresaken_by()
    /// - the class also handle the CSS components associated to HTML objects or to child objects
    ///   by mean of a css_library (usually only present in the root object of the tree)
    ///   and accessible by any children to store CSS class name+definition and refer to them when
    ///   needed. See store_css_library(), has_local_css_library(), lookup_css_library() and
    ///   new_css_library_available() to manage and access to a css_library global to the adoption
    ///   tree. See add_css_class() has_css_class() remove_css_class() clear_css_classes()
    ///   get_css_classes_as_a_set(), get_css_class_group(), get_css_classes() to assign one or more
    ///   css classes to a body_builder inherited object. See define_css_class_in_library() to
    ///   associate a CSS class name to its definition and store this in the closest css_library
    ///   available in the adoption tree (it is advised to only have one at the root of the tree)
    /// - objects of this class also produce HTML code referring to what should be displayed on
    ///   on the browser (get_body_part())
    /// \note more details about css. body_builder object can be associated to the name of one
    /// or more css_class or css_class_group at any time, those are just labels. The class
    /// definition is stored in a css_library. If a body_builder object have created
    /// a css_library (calling store_css_library()) all its adopted body_builder objects
    /// can access it and upload their class definitions using lookup_css_library().
    /// a class can be informed of the availability of a css_library overriding the
    /// protected method new_css_library_available() and implement there the css class
    /// definition they or their components have been associated to by mean of
    /// add_css_class(). Last from the outside of a class it is possible to define
    /// a css_class using the public method define_css_class_in_library(), but for
    /// this to work a css_library must be reachable in an ancestors (in regard to
    /// the adoption relationship) of this object.

class body_builder
{
public:
        /// constructor
    body_builder() { clear(); };

        /// copy constructor

	/// \note the created object cannot have any equivalent children as the source
	/// may have, because those are not memory managed by the body_builder class but
	/// by a derived class. This is thus the duty or derived class to rebuild
	/// a children equivalent tree if that makes sense. The newly created object
	/// should however stay orphaned (no parent), even if the source had a parent.
	/// This will ease copy-construction of derived class having body_builder inherited
	/// objects as fields and children.
    body_builder(const body_builder & ref);

        /// move constructor

	/// \note if move operation (constructor or assignment) have to be implemented in the
	/// future, care should be taken not to copy parent and children pointer values but to
	/// have source object foresaken() from parent and new one adopted() by the same parent,
	/// as source and moved objects have different addresses. Children adoption should also
	/// be taken care the same as parent children would change due to the move operation. Worse
	/// as the child-parent relationship is independent from the memory allocation responsibility
	/// of children objects, this is the inherited classes that should manage the move operation
	/// of the children to the their own field and reset the parent-child relationship accordingly
    body_builder(body_builder && ref) noexcept = delete;

        /// assignment operator drops all existing children

	/// \note see the note of the copy constructor
    body_builder & operator = (const body_builder & ref);

        /// move operator

	/// \note see the note of the move constructor
    body_builder & operator = (body_builder && ref) noexcept = delete;

        /// the (virtual) destructor
    virtual ~body_builder() { orphan_all_children(); unrecord_from_parent(); };




        /// set the root path at which this object will be located in the URL path
        ///
        /// \note this is only used if this object has no parent
    void set_prefix(const chemin & prefix);


        /// Common interface for classes that have to ask other body_builder to contribute in building their body_part()
        ///
        /// this call is not mandatory, but let a body_builder object the possibility to rely on the set of
        /// body_builder available protected routines to recursively build the body part from the list of
        /// adopted children. An object can only have one parent.
        /// \note this call semantic is that the adopted object becomes a child of the body_builder.
        /// The adopted child stays managed by its real parent, the adoption relationship builds a tree
        /// of dependant objects using one another to provide a body_part (see get_body_part() method).
        /// this tree is automatically broken when an object is destroyed, in that way all recorded
        /// children exist (the recorded addresses always points to real existing objects)
    void adopt(body_builder* obj);

	/// whether the object has currently a parent (= is adopted)
    bool is_adopted() const { return parent != nullptr; };

        /// Common interface for class that have to ask other body_builder to provide their own body_part()
        ///
        /// this call is not mandatory, it is the opposite action of adopt(). After this call
        /// the given object is no more sollicited to build a body part.
        /// \note if the requested object is not known an exception is thrown
    void foresake(body_builder* obj);

        /// ask for the object to become visible in HTML page or temporarily hidden

	/// \note an object which as its visible property set to false still has its inherited_get_body_part()
	/// method evaluated when calling get_body_part(), though get_body_part does not return its output.
	/// The reason of doing that way is to keep hidden components being updated under the scene and ready
	/// to show at any time.
    void set_visible(bool mode);

        /// returns the current visible status of the object
    bool get_visible() const { return visible; };

        /// set this object with a additional css_class (assuming it is defined in a css_library available for this object)
    void add_css_class(const std::string & name);

        /// set this object with an additional set of css_classes (assuming they are all defined in a css_library available for this object)
    void add_css_class(const css_class_group & cg);

        /// check the presence of the given class name
    bool has_css_class(const std::string & name) const;

        /// remove the provided css_class name from the list of css_class names this object has been assigned to
    void remove_css_class(const std::string & name);

        /// remove the provided css_class_group from the list
    void remove_css_class(const css_class_group & cg);

        /// clear the whole list of css_class names
    void clear_css_classes() { css_class_names.clear(); css_classes_have_changed(); };

        /// provide the list of css_class names that this object has been set with
    const std::set<std::string> & get_css_classes_as_a_set() const { return css_class_names; };

        /// provide the list of css_class names as a css_class_group
    css_class_group get_css_class_group() const;


        /// return the class_name as inlined css, suitable to be added in a html marker

        /// \note the returned string if not empty is of the form: class="<classname> <classname>..."
    std::string get_css_classes() const;

        /// add a css definition on a css_library reachable by this object
        /// \note so far this object does not use it, a call to add_css_class must also be done
        /// \note the provided css class name must not already exist (exception raised else)
    void define_css_class_in_library(const css_class & csscl);
    void define_css_class_in_library(const std::string & name, const css & cssdef);

        /// check whether a css_class of given name already exists in a reachable css_library
    bool is_css_class_defined_in_library(const std::string & name) const;

        /// ask the object to provide a part of the body to answer the request
        ///
        /// \param[in] path is the full path, but the path.index points to the asked object name
        /// \param[in] req is the request to be answered, the uri's path part targets the
        ///   choice of child to use for the sub-response
        /// \return the body part to be send in the response (html code or other depending
        ///  on the request) as defined by inherited_get_body_part()
        /// \note if path is an empty list only this object "body_builder" object can be
        /// used to answer, no child object can be invoked by mean of its name.
        /// \note when going down to the leaf of the tree, the path get shorter by removing
        /// the first items one at each step. Empty path means the object itself
        /// \note the inherited class should take the visible status in consideration when
        /// returning HTML from this call
    std::string get_body_part(const chemin & path,
			      const request & req);

	/// ignore my_body_part_has_changed() invoked from adopted children and myself

	/// some object (like html_statistics) are ever changing, but rely on some component
	/// that will trigger my_body_part_has_changed() to reflect the new value, leading the
	/// parent (here html_statistics) to be recorded as changed and the cycle is complete
	/// this process never ends. This call avoids propagating any future body_changed status
	/// toward the parents and set the caller as if it was a static, never changing, object.
    void ignore_body_changed_from_my_children(bool mode) { ignore_children_body_changed = mode; if(mode) body_changed = false; };

	/// get the current status of ignore_body_changed_from_my_children()
    bool get_ignore_body_changed_from_my_children() const { return ignore_children_body_changed; };

        /// ask for the implementation not to add a new line after this control
    void set_no_CR() { no_CR = true; };

protected:

	/// implementation of get_body_part() method for inherited classes

	/// \note this method defines what the class will return from the public method
	/// body_builder::get_body_part() which also make some housekeeping at body_builder level.
	/// In particular, the body_builder class record the "body_changed" status that is reset
	/// before calling inherited_get_body_part() and modified by the inherited class when they
	/// call the my_body_part_has_changed(). get_body_part() also caches the result of
	/// inherited_get_body_part() to avoid calling inherited_get_body_part() if for the same
	/// request it has not signaled (calling my_body_part_has_changed()) that for example an event
	/// lead its status to change and would require inherited_get_body_part() to be reevaluated
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) = 0;

	/// let a class record a change in what inherited_get_body_part() would return if it was called again with the same request

	/// method provided to inherited class to signal a change since the previous time inherited_get_body_part()
	/// has changed. This should not concern changes that are triggered by a call to inherited_get_body_part():
	/// \note A typical use case is when an body_builder is an actor on an event generated by another body_builder.
	/// If the first (actor) has its inherited_get_body_part() evaluated before the one of the event generator,
	/// when the inherited_get_body_part() is evaluated event leads to modify the first body_builder status and
	/// leads the already returned inherited_get_body_part() value to be wrong/obsolete. Having my_body_part_has_changed()
	/// invoked from the actor::on_event() method solves this dependency as get_body_part() will relaunch the
	/// evaluation of such objects that signaled they have changed.
    void my_body_part_has_changed();


	/// available for inherited class to be informed when their visibility changes
    virtual void my_visibility_has_changed() {};

	/// obtain the body_part changed status

	/// \note this is also set when adopted component triggered my_body_part_has_changed()
    bool has_my_body_part_changed() const { return body_changed; };

        /// return the path of 'this' according to its descent in the body_builder tree of adopted children
    chemin get_path() const;

        /// returns the name of 'this' if it has been adopted by a parent body_builder object
        ///
        /// \note, an empty string is returned if the object has not been adopted
    std::string get_recorded_name() const;


        /// this creates a css_library accessible from adopted objects to hold html class definitions

        /// \note a child object, directly or indirectly (grand child, aso) will be
        /// able to access and populate this css_library using the lookup_css_library() method
	/// \note avoid calling store_css_library() from a class constructor, this would break inherited
	/// class automatic be informed thanks to the new_css_library_available() method
    void store_css_library() { library_asked = true; };

	/// return true if this object has a css_library locally stored (not from a adopter object)
    bool has_local_css_library() const { return library_asked || bool(library); };

        /// lookup toward registered parent body_builder object for the closest stored css_library

        /// \note if neither the present object nor any of its parent stores an css_library,
        /// the returned unique_ptr is false (points to nullptr), else it points to the found css_library
    std::unique_ptr<css_library> & lookup_css_library() const;

        /// let a parent obtain the body part from one of its children given its official name and seen the path of the request
        ///
        /// \param[in] path is the path exactly as received from the get_body_part call:
        /// the first member is the name of a child object.
        /// \param[in] req is the request exactly as received from the get_body_part call
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
        /// \param[in] path is the body_builder path, it can be empty. If not the front member is poped from the target
        /// even if the poped part of the path does not match the name of the consulted child object
        /// \param[in] req this is the request exactly as received from the get_body_part call
        /// \note if the object is not visible, this call will return an empty string
    std::string get_body_part_from_all_children(const chemin & path,
                                                const request & req);

        /// For inherited classes, called when the path has changed,

        /// \note tipically this is when this object has been adopted or foresaken
    virtual void path_has_changed() {};

        /// Be informed that a new child has been adopted
    virtual void has_adopted(body_builder *obj) {};

        /// Be informed that a child is about to be foresaken
    virtual void will_foresake(body_builder *obj) {};

        /// Be informed that we have been adopted by obj, our new parent
    virtual void has_been_adopted_by(body_builder *obj) {};

        /// Be informed that we are about to be foresaken by obj, our soon former parent
    virtual void will_be_foresaken_by(body_builder *obj) {};

	/// Be informed about css class modification
    virtual void css_classes_have_changed() {};

        /// this is a trigger, ran when a css_library becomes available in a parent or "this"
        /// \note this may also be triggered upon adoption by a object having acces to a css_library
    virtual void new_css_library_available() {};

        /// access to adopted childs
    unsigned int size() const { return order.size(); };

        /// access to adopted childs
    body_builder *operator[] (unsigned int i) { return order[i]; };

	/// access to the closest ancestor (parent) matching (dynamic_cast) the provided type

	/// \param[in] ancestor point to the found ancestor if found (points to nullptr else)
    template <class T> void closest_ancestor_of_type(T* & ancestor)
    {
	body_builder* ptr = parent;

	do
	{
	    if(ptr != nullptr)
	    {
		ancestor = dynamic_cast<T*>(ptr);
		ptr = ptr->parent;
	    }
	    else
		ancestor = nullptr;
	}
	while(ptr != nullptr && ancestor == nullptr);
    }


        /// orphan all adopted children
    void orphan_all_children();

        /// true if it has been requested no to add Carriage Return after the HTML object
    bool get_no_CR() const { return no_CR; };


private:
    bool visible;                                       ///< whether this object is visible or not
    chemin x_prefix;                                    ///< path of this object
    bool no_CR;                                         ///< whether inherited class implementation should avoid adding a CR at end of HTML produced body part
    body_builder* parent;                               ///< our parent if we get adopted
    std::vector<body_builder*> order;                   ///< children by order or adoption
    std::map<std::string, body_builder*> children;      ///< children and their name
    std::map<body_builder*, std::string> revert_child;  ///< revert map to get name of a child
    chemin last_body_path;                              ///< last path value provided to get_body_part()
    uri last_body_req_uri;                              ///< last req uri value provided to get_body_part()
    std::string last_body_req_body;                     ///< last req body value provided to get_body_part()
    std::string last_body_part;                         ///< last return of inherited_get_body_part()
    bool library_asked;                                 ///< whether store_css_library() has been called,
	                                                ///< this will trigger creation of css_library from get_body_part,
	                                                ///< to allow store_css_library() being invoked from constructors of inherited classes
    std::unique_ptr<css_library> library;               ///< css library if stored by this object
    std::set<std::string> css_class_names;              ///< list of CSS class that apply to this object
    bool body_changed;                                  ///< change status of inherited_get_body_part()/get_body_part()
    bool ignore_children_body_changed;                  ///< ignore and propagate body_changed notif from adopted children

        /// unrecord 'this' from its parent as a adopted child
    void unrecord_from_parent();

        /// inform children and all their descendant children that the path has changed calling their path_has_changed() method
    void recursive_path_has_changed();

        /// ask inherited class to provide its css_classes
    void recursive_new_css_library_available();

        /// (re)initialize fields to default sane values
    void clear();

	/// create a css_library if it has been requested earlier
    void create_css_lib_if_needed();

	/// flush cached body_part for me and all my children
    void flush_all_cached_body_part();

	/// fetch cached body_part if the object has not changed since previous call

	/// \note several point have to be addresses:
	/// - an body_builder object can change after is inherited_get_body_part() has return
	///   due to the fact another object generated an event, which my_body_part_has_changed() keep trace of
	/// - some objects need not to be evaluated twice (toggle objects) to not avoid undoing
	///   the action that was request -> this must be avoided by using different events/objects to do an undo an action
	///   even if they have the same appearence and place
	/// - some other objects do not change but need to be evaluated at any time (html_button)
	///   to perform the desired action.
    std::string get_body_part_or_cache(const chemin & path,
				       const request & req);

};

#endif
