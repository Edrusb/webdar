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

#ifndef HTML_FORM_OVERWRITE_BASE_CRITERIUM_HPP
#define HTML_FORM_OVERWRITE_BASE_CRITERIUM_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_overwrite_criterium.hpp"
#include "actor.hpp"
#include "body_builder.hpp"
#include "html_form_input.hpp"
#include "html_datetime.hpp"
#include "html_form_select.hpp"
#include "html_form_fieldset.hpp"
#include "jsoner.hpp"
#include "events.hpp"

    /// html component to be adopted by an html_form that implements atomic/base actions for overwriting policies

class html_form_overwrite_base_criterium: public html_overwrite_criterium,
                                          public actor,
                                          public jsoner,
					  public events
{
public:

	// events

    static const std::string changed;

	// base modes for see set_mode()

    static constexpr const char* crit_in_place_is_inode = "in place entry is an inode";
    static constexpr const char* crit_in_place_is_dir = "in place entry is a directory";
    static constexpr const char* crit_in_place_is_file = "in place entry is a plain file";
    static constexpr const char* crit_in_place_is_hardlinked_inode = "in place entry is a hard linked inode";
    static constexpr const char* crit_in_place_is_new_hardlinked_inode = "in place entry is a hard linked inode we meet for the first time";
    static constexpr const char* crit_in_place_data_more_recent = "in place inode has more recent data";
    static constexpr const char* crit_in_place_data_more_recent_or_equal_to = "in place inode a data more recent than or equal to: ";
    static constexpr const char* crit_in_place_data_bigger = "both are plain files and in place one is bigger file";
    static constexpr const char* crit_in_place_data_saved = "in place has its data saved or is not an inode";
    static constexpr const char* crit_in_place_data_dirty = "in place entry is a dirty file";
    static constexpr const char* crit_in_place_data_sparse = "in place entry is a sparse file";
    static constexpr const char* crit_in_place_has_delta_sig = "in place entry has a delta signature attached";
    static constexpr const char* crit_same_inode_data = "both entry are of the same type and have the same metadata";
    static constexpr const char* crit_in_place_EA_present = "in place entry has Extended Attributes";
    static constexpr const char* crit_in_place_EA_more_recent = "in place entry has more recent Extended Attributes";
    static constexpr const char* crit_in_place_EA_more_recent_or_equal_to = "in place entry has EA more recent than or equal to:";
    static constexpr const char* crit_in_place_more_EA = "in place entry has more Extended Attributes";
    static constexpr const char* crit_in_place_EA_bigger = "in place entry has bigger Extended Attributes";
    static constexpr const char* crit_in_place_EA_saved = "in place entry has Extended Attributes saved";
    static constexpr const char* crit_same_type = "both entries are of the same type";


    html_form_overwrite_base_criterium();
    html_form_overwrite_base_criterium(const html_form_overwrite_base_criterium & ref) = default;
    html_form_overwrite_base_criterium(html_form_overwrite_base_criterium && ref) noexcept = default;
    html_form_overwrite_base_criterium & operator = (const html_form_overwrite_base_criterium & ref) = default;
    html_form_overwrite_base_criterium & operator = (html_form_overwrite_base_criterium && ref) noexcept = default;
    ~html_form_overwrite_base_criterium() = default;

	/// set the base mode (use crit_* static char* above)

	/// \note this call is to programmatically setup a component, normal way to setup is from Web user interaction
    void set_mode(const std::string & mode) { base.set_selected_id(mode); };

	/// set invert value

	/// \note this call is to programmatically setup a component, normal way to setup is from Web user interaction
    void set_invert(bool val) { invert.set_value_as_bool(val); };

	/// set invert value

	/// \note this call is to programmatically setup a component, normal way to setup is from Web user interaction
    void set_negate(bool val) { negate.set_value_as_bool(val); };

	/// set date

	/// \note this call is to programmatically setup a component, normal way to setup is from Web user interaction
    void set_date(const libdar::infinint & val) { date.set_value(val); };

	/// set hourshift

	/// \note this call is to programmatically setup a component, normal way to setup is from Web user interaction
    void set_hourshift(unsigned int shift) { hourshift.set_value_as_int(shift); };


        /// obtain the crit_base_criterium object for libdar option
    virtual std::unique_ptr<libdar::criterium> get_overwriting_criterium() const override;

        /// inherited from jsoner
    virtual void load_json(const json & source) override;

        /// inherited from jsoner
    virtual json save_json() const override;

        /// inherited from jsoner
    virtual void clear_json() override;

        /// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:

        /// inherited from body_builder from html_overwrite_criterium
    virtual std::string inherited_get_body_part(const chemin & path,
                                                const request & req) override;

private:
    html_form_fieldset crit_fs; ///< wrapps all html form components of this class
    html_form_input negate; ///< checkbox
    html_form_input invert; ///< checkbox
    html_form_select base;  ///< selection of the base criterium
    html_datetime date; ///< for date related criterium
    html_form_input hourshift; ///< for date related criterium

    bool trigger_change; ///< whether a change occurred in subcomponent during a inherited_get_body_part_evaluation, json_load or json_clear

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_form_overwrite_base_criterium";

    static constexpr const char* jlabel_negate = "negate";
    static constexpr const char* jlabel_invert = "invert";
    static constexpr const char* jlabel_base = "base";
    static constexpr const char* jlabel_date = "date";
    static constexpr const char* jlabel_hourshift = "hourshift";


};

#endif
