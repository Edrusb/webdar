#ifndef HTML_LEVEL_HPP
#define HTML_LEVEL_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <list>

    // webdar headers
#include "body_builder.hpp"

    /// class html_level is a pure virtual class
    ///
    /// it implements the management of a mixed list of object and static html text
    /// and provides method to read this list. This class does not implement the
    /// get_body_part() pure virtual method of its body_builder ancestor class
    /// this is left to its inherited classes

class html_level : public body_builder
{
public:
    html_level();
    html_level(const html_level & ref) { throw WEBDAR_BUG; };
    const html_level & operator = (const html_level & ref) { throw WEBDAR_BUG; };

	/// used in concurrency with body_builder::adopt()
    void adopt_static_html(const std::string & html);

	/// clear all adopted data
    void clear();

protected:
	/// inherited from body_builder
    virtual void has_been_adopted(body_builder *obj);

	/// inherited from body_builder
    virtual void has_been_foresaken(body_builder *obj);

    struct bundle
    {
	body_builder *obj;       //< if set to NULL, using static_text instead
	std::string static_text; //< used if obj is NULL
	bundle() { obj = NULL; };
    };

    void reset_read_next() { nxt = table.begin(); };
    bool read_next(bundle & bdl); //< return false if bdl could not be set

	/// this is an alternative of using read_next
	///
	/// \note to be used when all children body_part are needed as a single block of text
    std::string get_body_part_from_children_as_a_block(const chemin & path,
						       const request & req);
	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    std::list<bundle>::iterator nxt;
    std::list<bundle> table;
};

#endif
