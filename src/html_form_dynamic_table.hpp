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

#ifndef HTML_FORM_DYNAMIC_TABLE_HPP
#define HTML_FORM_DYNAMIC_TABLE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <memory>
#include <string>

    // webdar headers
#include "actor.hpp"
#include "events.hpp"
#include "html_div.hpp"
#include "html_table.hpp"
#include "html_button.hpp"
#include "html_text.hpp"
#include "html_form_input.hpp"
#include "html_form_select.hpp"

    /// class html_form_dynamic_table_object_provider

    /// is an interface for objets using an html_form_dynamic_table. This component
    /// will call back the recorded provider object when a new entry is asked from web
    /// interface to be added in the table.
    /// \note most of the time this will be the owner of a html_form_dynamic_table
    /// that will register itself as html_form_dynamic_table_object_provider to
    /// his owned dynamic table object


class html_form_dynamic_table_object_provider
{
public:
    virtual std::unique_ptr<body_builder> provide_object_of_type(unsigned int num, const std::string & context) const = 0;
};


    /// class html_form_dynamic_table web user dynamically add new lines in a table

    /// The possible type of lines of the table are defined by the developper user of this class
    /// but this is the web user that defined the table content based on the available types
    ///
    /// this html components is consitued of:

    /** \verbatim
	+----html_table-----------------------------+-------+
	|       |                                   |       |
	|+-----+|+-------------------------------+  |+-+    |
	||label|||body_builder objet             |  || | del|
	|+-----+|+--------------------------------  |+-+    |
	|+-----+|+-------------------------------+  |+-+    |
	||label|||body_builder objet             |  || | del|
	|+-----+|+--------------------------------  |+-+    |
	|+-----+|+-------------------------------+  |+-+    |
	||label|||body_builder objet             |  || | del|
	|+-----+|+--------------------------------  |+-+    |
	|+-----+|+-------------------------------+  |+-+    |
	||label|||body_builder objet             |  || | del|
	|+-----+|+--------------------------------  |+-+    |
	|+-----+|+-------------------------------+  |+-+    |
	||label|||body_builder objet             |  || | del|
	|+-----+|+--------------------------------  |+-+    |
	+-------+-----------------------------------+-------+

	+---------------------+
	|  add drop down list | (or html_button if only one type to add
	+---------------------+ (option to position the button before the table or after)
	\endverbatim **/

    /// \note this component must be adopted by an html_form
    /// \note the programmer should set an object provider
    /// to create object component of the requested type by the
    /// user.


class html_form_dynamic_table : public html_div, public actor, public events
{
private:
    struct line; // referred before the public part for the ::iterator nested class

public:
    static const std::string changed;  /// default event name for all object of this clas

        /// constructor
    html_form_dynamic_table(bool has_left_labels,            ///< whether to display label on a left column
                            bool selector_below,             ///< whether the selector is above or below the table
                            const std::string & adder_text,  ///< fixed label shown beside the add drop list
                            const std::string & adder_default_choice ///< default choice in the drop list
	);

    html_form_dynamic_table(const html_form_dynamic_table & ref) = delete;
    html_form_dynamic_table(html_form_dynamic_table && ref) noexcept = delete;
    html_form_dynamic_table & operator = (const html_form_dynamic_table & ref) = delete;
    html_form_dynamic_table & operator = (html_form_dynamic_table && ref) noexcept = delete;
    ~html_form_dynamic_table() = default;

    class iterator
    {
    public:
        iterator() {};
        iterator(const iterator & ref) = default;
        iterator(iterator && ref) = default;
        iterator & operator = (const iterator & ref) = default;
        iterator & operator = (iterator && ref) = default;
        ~iterator() = default;

        iterator & operator ++ (signed int n) { ++ptr; return *this; };
        iterator operator ++ () { iterator ret; ret.ptr = ptr; ++ptr; return ret; };
        bool operator == (const iterator & ref) const { return ptr == ref.ptr; };
        bool operator != (const iterator & ref) const { return ptr != ref.ptr; };
        std::shared_ptr<html_text> get_left_label() const { return ptr->left_label; };
        unsigned int get_object_type() const { return ptr->object_type_index; };
        std::shared_ptr<body_builder> get_object() const { return ptr->dynobj; };

    private:
        std::list<line>::const_iterator ptr;

        friend html_form_dynamic_table;
    };

    iterator begin() const { iterator ret; ret.ptr = table_content.begin(); return ret; };
    iterator end() const { iterator ret; ret.ptr = table_content.end(); return ret; };
    iterator last() const { iterator ret; ret.ptr = prev(table_content.end()); return ret; };

	/// whether the current dynamic_table is empty
    bool empty() const { return table_content.empty(); };

	/// define a object provider

	///\param[in] provider the object provider.

	/// when needed by this dynamic table, the object provider will be asked
	/// to create a new object of type "num", the argument of
	/// html_form_dynamic_table_object_provider::provide_object_of_type()
	/// for num = 0 the object type provided by the first call to
	/// add_obj_type(), num = 1 correspond to the type of object
	/// which label has been provide by the second time add_obj_type()
	/// was called.

	/// *before* adding types of objects that will be proposed to the web user,
	/// an object provider (and only one) must be recorded calling this method
	/// \note the object provider is not memory managed by the "this"
	/// object and must survive during the whole life of "this".
	/// \note only one and same provider must address all the possible type of
	/// object to build. Any previously added provider is replaced by the one
	/// give here.
    void set_obj_type_provider(const html_form_dynamic_table_object_provider* provider);

	/// get the current object provider
    const html_form_dynamic_table_object_provider* get_current_provider() const { return my_provider; };

	/// set context passed to the object provider

	/// \note if context is not defined, an empty string is passed
	/// to the object provider
    void set_obj_type_context(const std::string & ctxt) { context = ctxt; };

        /// add a new object type to be proposed to the user from the "adder" selector

        /// \param[in] label the column number for this object (starts at zero)
    void add_obj_type(const std::string & label);

	/// return the number of object type available to the user
    unsigned int obj_type_size() const { return adder.size() - 1; };

        /// clear table content
    void clear() { table_content.clear(); del_event_to_content.clear(); events_to_delete.clear(); };

	/// manually add an line in the table with the object type give in number

	/// \note this is only needed to build programmatically the dynamic_table, but
	/// a dynamic_table is more than a table, it is dynamic in the sense that this
	/// through user action in web interface that normally this method is invoked
	/// to add (but also remove) lines to the table
    void add_line(unsigned int typenum);


        /// inherited from actor
    virtual void on_event(const std::string & event_name) override;

        /// exposed css method from underlying html_table
    void css_border_collapsed(bool mode) { table.css_border_collapsed(mode); };

        /// exposed css method from underlying html_table
    void set_css_class_first_row(const std::string & val) { table.set_css_class_first_row(val); };

        /// exposed css method from underlying html_table
    void set_css_class_first_row() { table.set_css_class_first_row(); };

        /// exposed css method from underlying html_table
    void set_css_class_first_column(const std::string & val) { table.set_css_class_first_column(val); };

        /// exposed css method from underlying html_table
    void set_css_class_first_column() { table.set_css_class_first_column(); };

        /// exposed css method from underlying html_table
    void set_css_class_cells(const std::string & val) { table.set_css_class_cells(val); };

        /// exposed css method from underlying html_table
    void set_css_class_cells() { table.set_css_class_cells(); };


protected:
        /// inherited methods from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
                                                const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:

        // events
    static constexpr const char* new_line_to_add = "new_line";

        /// objects adopted for a given line of the "table" and managed by table_content
    struct line
    {
        line() { left_label.reset(); dynobj.reset(); del.reset(); };

        std::shared_ptr<html_text> left_label;
        unsigned int object_type_index; ///< index of the brother used in list_of_types
        std::shared_ptr<body_builder> dynobj;
        std::unique_ptr<html_form_input> del;
    };

    const html_form_dynamic_table_object_provider* my_provider; ///< the object provider to use
    std::string context;                 ///< the context string passed when to the object provider
    std::list<line> table_content;       ///< manages the objects adopted by "table"
    unsigned int event_del_count;        ///< used to build new event name for each new "delete" button
    std::map<std::string, std::list<line>::iterator> del_event_to_content; /// find the table_content entry corresponding to a given event
    std::deque<std::string> events_to_delete; ///< temporary list of events pending for deletion

    bool left_label;
    html_table table;
    html_form_select adder;

    void del_line(const std::string & event_name);
    void purge_to_delete();

    friend html_form_dynamic_table::iterator;
};

#endif
