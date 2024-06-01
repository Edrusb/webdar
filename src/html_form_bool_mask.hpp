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

#ifndef HTML_FORM_BOOL_MASK_HPP
#define HTML_FORM_BOOL_MASK_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <memory>
#include <list>
#include <map>

    // webdar headers
#include "html_mask.hpp"
#include "actor.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_select.hpp"
#include "html_table.hpp"
#include "html_form_input.hpp"
#include "html_form.hpp"


    /// class html_form_bool_mask provide mean to combines with OR and AND different html_masks

    /// this html components is consitued of:
    ///
    /// +----html_fieldset----------------------------------+
    /// |                                                   |
    /// | logical combination:  +----------------+          |
    /// |                       |dropdown list   |          |
    /// |                       +----------------+          |
    /// |        +-------------------------------+-----+    |
    /// |        | table of masks                | DEL |    |
    /// |        +-------------------------------+-----+    |
    /// |                                                   |
    /// |         [ add new mask dropdown list]             |
    /// |                                                   |
    /// +---------------------------------------------------+
    ///
    /// \note: this component does must have an html_form object
    /// as parent or ancestor for validation to be processed and
    /// changes to be recorded. Avoiding including the html_form
    /// in that component let it be part of more complex masks
    /// like *and* and *or* combinasons.


class html_form_bool_mask : public html_mask, public actor
{
public:
    html_form_bool_mask(bool include_html_form = true);
    html_form_bool_mask(const html_form_bool_mask & ref) = default;
    html_form_bool_mask(html_form_bool_mask && ref) noexcept = default;
    html_form_bool_mask & operator = (const html_form_bool_mask & ref) = default;
    html_form_bool_mask & operator = (html_form_bool_mask && ref) noexcept = default;
    ~html_form_bool_mask() = default;

	/// inherited from html_mask
    std::unique_ptr<libdar::mask> get_mask() const override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;


protected:
	/// inherited methods from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    static constexpr const char* new_mask_to_add = "new_mask";
    static constexpr const char* and_op = "and";
    static constexpr const char* or_op = "or";
    static constexpr const char* type_undefined = "undefined";
    static constexpr const char* type_filename = "filename";
    static constexpr const char* type_bool = "bool";

    struct entry
    {
	entry() { mask.reset(); del.reset(); };
	entry && vampire(entry & ref) && { mask.reset(ref.mask.release()); del.reset(ref.del.release()); return std::move(*this); };

	std::unique_ptr<html_mask> mask;
	std::unique_ptr<html_form_input> del;
    };

    html_form_fieldset fs;
    html_form_select mask_type;
    html_table table;
    html_form_select adder;
    html_form papillotte;

    std::list<entry> table_content;
    std::map<std::string, std::list<entry>::iterator> del_event_to_content;
    unsigned int event_del_count;
    std::deque<std::string> events_to_delete; ///< components that will be deleted once the inherited_get_body_part will have finished
	/// \note cannot delete an object (mask and del) from an on_event() actor method triggered by the object itself

    void add_mask(const std::string & mask_type);
    void del_mask(const std::string & event_name);
    void purge_to_delete();

};

#endif
