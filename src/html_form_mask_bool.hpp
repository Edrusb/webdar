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

#ifndef HTML_FORM_MASK_BOOL_HPP
#define HTML_FORM_MASK_BOOL_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <memory>
#include <list>
#include <map>
#include <deque>

    // webdar headers
#include "html_mask.hpp"
#include "actor.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_select.hpp"
#include "html_table.hpp"
#include "html_form_input.hpp"
#include "html_form.hpp"
#include "html_text.hpp"


    /// class html_form_mask_bool provide mean to combines with OR and AND different html_masks

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
    /// \note this component includes an html_form by default
    /// (see constructor argument), but should not when used
    /// recursively from another html_form_mask_bool


class html_form_mask_bool : public html_mask, public actor
{
public:
    html_form_mask_bool();
    html_form_mask_bool(const html_form_mask_bool & ref);
    html_form_mask_bool(html_form_mask_bool && ref) noexcept = delete;
    html_form_mask_bool & operator = (const html_form_mask_bool & ref) = delete;
    html_form_mask_bool & operator = (html_form_mask_bool && ref) noexcept = delete;
    ~html_form_mask_bool() = default;

	/// component that will provided as possible to add in the logic operation of the current mask
    void add_mask_type(const std::string & label, const html_mask & tobecloned);

	/// add recursion with object of the same configuration as "*this" in the list of available masks

	/// \note if you want other masks to be available recursively this call must be invoked
	/// after all add_mask_type() with two arguments.
    void add_mask_myself(const std::string & label);


	/// inherited from html_mask
    virtual void set_root_prefix(const libdar::path & x_prefix) override;


	/// inherited from html_mask
    virtual std::unique_ptr<libdar::mask> get_mask() const override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// clone() implementation
    MASK_CLONER_MACRO;


protected:
	/// inherited methods from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;


private:
    static constexpr const char* new_mask_to_add = "new_mask";
    static constexpr const char* and_op = "and_op";
    static constexpr const char* or_op = "or_op";

    static constexpr const char* bool_changed_event = "bool_changed";
    static constexpr const char* css_class_bool_text = "html_form_mask_bool_bool_text";

	//

    struct available_mask
    {
	std::string label;
	std::unique_ptr<html_mask> mask_type; // mask_type is not set to refer to this html_form_mask_bool

	available_mask(const std::string & label,
		       const html_mask & tobecloned);
	available_mask(const std::string & label); // to refer to this html_form_mask_bool
	available_mask(available_mask && ref) noexcept;
	available_mask(const available_mask & arg);
    };

    std::deque<available_mask> list_of_mask_types;

	//

    struct entry
    {
	entry() { mask.reset(); del.reset(); };

	std::unique_ptr<html_text> logic;  ///< text displaying "and" or "or"
	std::unique_ptr<html_mask> mask;   ///< the mask
	std::unique_ptr<html_form_input> del;  ///< the button to delete this mask
    };

    std::list<entry> table_content;

	//

    html_form_fieldset fs;
    html_form_select mask_type;
    html_table table;
    html_form_select adder;

    std::map<std::string, std::list<entry>::iterator> del_event_to_content;
    unsigned int event_del_count;
    std::deque<std::string> events_to_delete; ///< components that will be deleted once the inherited_get_body_part will have finished
	/// \note cannot delete an object (mask and del) from an on_event() actor method triggered by the object itself
    std::string current_bool_mode; ///< currently displayed logic in table
    unsigned int current_table_size; ///< size at which the displayed logic was last updated
    libdar::path root_prefix;

    void init();
    void add_mask(unsigned int num); ///< \param[in] num is the index in list_of_mask_types
    void del_mask(const std::string & event_name);
    void purge_to_delete();
    std::string bool_op_to_name(const std::string & op);
    void update_table_content_logic(); // update labels in the first column in regard to the current AND/OR selected logic
};

#endif
