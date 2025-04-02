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

#ifndef HTML_SIZE_UNIT_HPP
#define HTML_SIZE_UNIT_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "html_form_select.hpp"
#include "html_span.hpp"

    /// html component in a form a couple of dropdown list for user to define a size in byte or larger units

class html_size_unit : public html_span, public actor, public events
{
public:
    static const std::string changed;

    html_size_unit();
    html_size_unit(const html_size_unit & ref) = delete;
    html_size_unit(html_size_unit && ref) noexcept = delete;
    html_size_unit & operator = (const html_size_unit & ref) = delete;
    html_size_unit & operator = (html_size_unit && ref) noexcept = delete;
    ~html_size_unit() = default;

	/// return the unit factor as integer
    libdar::infinint get_value() const;

	/// return the unit label
    std::string get_string() const { return unit.get_selected_id(); };

	/// get the max base unit index (index starts at zero)
    unsigned int get_max_base_unit_index() const { return SI_mode.num_choices() - 1; };

	/// get current base index
    unsigned int get_current_base_index() const { return SI_mode.get_selected_num(); };

	/// get value of the base unit of given index

	/// \note base units are stored in ascending order with the index
    libdar::infinint get_base_unit_value(unsigned int index) const;

	/// get the max power available for the base unit of given index

	/// \note power 0 means factor is "base_unit^0" (which is 1)... and so on.
    unsigned int get_max_power_for_base_unit(unsigned int index) const;

	/// get the current power selected
    unsigned int get_current_power_for_base_unit() const { return unit.get_selected_num(); };


	/// change the selected base unit and factor programmatically
    void set_unit_and_ratio_indexes(unsigned int base_index, unsigned int power_index);

	/// whether the HTML control is enable or disabled
    void set_enabled(bool val) { unit.set_enabled(val); SI_mode.set_enabled(val); };


	/// actor inheritance
    virtual void on_event(const std::string & event_name) override;


private:
    static constexpr const char* mode_changed = "SI_mode changed";

    bool manual_change;
    html_form_select unit;
    html_form_select SI_mode;

    void set_fields();
};

#endif
