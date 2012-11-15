#ifndef BODY_BUILDER_HPP
#define BODY_BUILDER_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <map>

    // webdar headers
#include "request.hpp"
#include "css.hpp"
#include "chemin.hpp"


class body_builder : public css
{
public:
	/// constructor
    body_builder() { parent = NULL; children.clear(); revert_child.clear(); };

	/// avoiding copy constructor use
    body_builder(const body_builder & ref) { throw WEBDAR_BUG; };

	/// avoiding assignment operator are fine here
    const body_builder & operator = (const body_builder & ref) { throw WEBDAR_BUG; };

	/// the (virtual) destructor
    virtual ~body_builder() { clear_and_delete_children(); unrecord_from_parent(); };


	/// set the root path at which this object will be located in the URL path
	///
	/// \note this is only used if this object has no parent
    void set_prefix(const chemin & prefix) { x_prefix = prefix; };


	/// Common interface for class that can/must contain other body_builder to provide a body_part()
	///
	/// this call is not mandatory, but must be overwritten by inherited class in order to be used
	/// \note this call semantic is that the given object is passed to the body_builder, which is in charge
	/// of its memory management (in particular at body_builder destruction) or un until its returned to the caller
    virtual void give(body_builder *obj) { throw WEBDAR_BUG; };

	/// Common interface for class the can/must contain other body_builder to provide a body_part()
	///
	/// this call is not mandatory, but must be overwritten by inherided class in order to be used
	/// \note this call semantic is that the given object is returned the caller, this body_builder is no more
	/// in charge of its memory management.
	/// \note if the requested object is not known an exception is thrown
    virtual void take_back(body_builder *obj) { throw WEBDAR_BUG; };


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
    virtual std::string get_body_part(const chemin & path,
				      const request & req) = 0;


protected:

	/// return the path of 'this' according to its descent in the body_builder tree
    chemin get_path() const;

	/// returns the name of 'this' if it has been recorded by a parent body_builder object
	///
	/// \note, an empty string is returned if the object has not been recorded by a parent body_builder object
    std::string get_recorded_name() const;


	/// record a object as child of this object. This gives an official name to this object as part of a tree heirarchy
	///
	/// \param[in] obj the body_builder inherited class object's address
	/// \return the name given to that object (which one will be used to build the path)
	/// \note this call DOES transfer responsibility about given child memory management. In particular
	/// if this object is not "unrecorded" it will be unrecorded *and* deleted by the body_builder destructor
	/// \note This call builds a hierarchical tree of body_builder that is used in request URI:
	/// After the session_ID in the path, takes place the name of the object to ask an answer
	/// from in return to the requested URI.
    std::string record_child(body_builder *obj);


	/// remove the name associated to this object
	///
	/// \param[in] the child's address to unrecord (the object is not touched, only its
	/// name is deleted from the base, the object responsibility passes back to the caller's
	/// \note if no direct child has such a name an exception exception_range is thrown
    void unrecord_child(body_builder *obj);

	/// unrecord and destroy all children recusively
    void clear_and_delete_children();

	/// let a parent obtain the body part from one of its children given its official name
	///
	/// \param[in] path, this is the path exactly as received from the get_body_part call:
	/// the first member is the name of a child object.
	/// \param[in] req, this is the request exactly as received from the get_body_part call
	/// \note it is typically called from the parent get_body_part() method
	/// \note if path is an empty list, the call fails (throwing an exception) as no
	/// no further name is available to find an child object to return its body part.
	/// In consequence, the caller must check whether the path is empty() or not before
	/// invoking this method
    std::string get_body_part_from_target_child(const chemin & path,
						const request & req);

	/// let a parent obtain the body part from all children in the order the have been recorded
	///
	/// \param[in] path, this is the path, it can be empty. If not the front member is poped from the target
	/// even if the poped part of the path does not match the name of the consulted child object
	/// \param[in] req, this is the request exactly as received from the get_body_part call
    std::string get_body_part_from_all_children(const chemin & path,
						const request & req);

private:
    chemin x_prefix;
    body_builder *parent;
    std::list<body_builder *> order;
    std::map<std::string, body_builder *> children;
    std::map<body_builder *, std::string> revert_child;

    void unrecord_from_parent();
};

#endif
