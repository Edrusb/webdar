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
extern "C"
{

}

    // C++ system header files
#include <memory>
#include <string>

    // webdar headers
#include "actor.hpp"
#include "events.hpp"
#include "body_builder.hpp"
#include "html_table.hpp"
#include "html_button.hpp"
#include "html_text.hpp"
#include "html_form_input.hpp"
#include "html_form_select.hpp"

    /// class html_form_dynamic_table web user dynamically add new lines in a table

    /// The lines of the table are defined by the developper user of this class
    /// providing a set of body_builder objets which class has the make_brother()
    /// method implemented to create a brandh-new object of the same inherited
    /// class as the ones given as reference to define the table's line content
    ///
    /// this html components is consitued of:
    ///
    /// +----html_table-----------------------------+-------+
    /// |       |                                   |       |
    /// |+-----+|+-------------------------------+  |+-+    |
    /// ||label|||body_builder objet             |  || | del|
    /// |+-----+|+--------------------------------  |+-+    |
    /// |+-----+|+-------------------------------+  |+-+    |
    /// ||label|||body_builder objet             |  || | del|
    /// |+-----+|+--------------------------------  |+-+    |
    /// |+-----+|+-------------------------------+  |+-+    |
    /// ||label|||body_builder objet             |  || | del|
    /// |+-----+|+--------------------------------  |+-+    |
    /// |+-----+|+-------------------------------+  |+-+    |
    /// ||label|||body_builder objet             |  || | del|
    /// |+-----+|+--------------------------------  |+-+    |
    /// |+-----+|+-------------------------------+  |+-+    |
    /// ||label|||body_builder objet             |  || | del|
    /// |+-----+|+--------------------------------  |+-+    |
    /// +-------+-----------------------------------+-------+
    ///
    /// +---------------------+
    /// |  add drop down list | (or html_button if only one type to add
    /// +---------------------+ (option to position the button before the table or after)
    ///
    ///
    /// \note this component must be adopted by an html_form
    /// \note the programmer should provide the components type
    /// that will consititue each new line calling add_type()
    /// and the provided body_builder objects must have their
    /// method make_brother() re-defined to provide another object
    /// (possibily a brand-new one or a copy constructed one if necessary)
    /// of the exact same class (not of a parent class).

class html_form_dynamic_table : public body_builder, public actor, public events
{
private:
    struct line; // referred before the public part for the ::iterator nested class

public:
    static const std::string changed;  /// default event name for all object of this clas

        /// constructor
    html_form_dynamic_table(bool has_left_labels,
                            bool selector_below,
                            const std::string & adder_text,  ///< fixed label shown beside the add drop list
                            const std::string & adder_default_choice); ///< default choice in the drop list

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
    bool empty() const { return table_content.empty(); };

        /// add a new object type to be proposed to the user from the "adder" selector

        /// \param[in] column the column number for this object (starts at zero)
        /// \param[in,out] obj pointer to object that will be passed to the html_form_dynamic_table
    void add_obj_type(const std::string & label, const body_builder & obj);

        /// clear table content
    void clear() { table_content.clear(); del_event_to_content.clear(); events_to_delete.clear(); };

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

    std::deque<std::unique_ptr<body_builder> > list_of_types; ///< available types of objects
    std::list<line> table_content;       ///< manages the objects adopted by "table"
    unsigned int event_del_count;        ///< used to build new event name for each new "delete" button
    std::map<std::string, std::list<line>::iterator> del_event_to_content; /// find the table_content entry corresponding to a given event
    std::deque<std::string> events_to_delete; ///< temporary list of events pending for deletion

    bool left_label;
    html_table table;
    html_form_select adder;

    void add_line(unsigned int typenum);
    void del_line(const std::string & event_name);
    void purge_to_delete();

    friend html_form_dynamic_table::iterator;
};

#endif
