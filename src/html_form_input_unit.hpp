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

#ifndef HTML_FORM_INPUT_UNIT_HPP
#define HTML_FORM_INPUT_UNIT_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "html_form_input.hpp"
#include "html_size_unit.hpp"
#include "events.hpp"
#include "actor.hpp"

    /// class html_form_input_unit implements HTML input::number feature with added unit box

    /// it is expected to be adopted by an html_form directory
    /// or through an html_form_fieldset, to provide a mean for
    /// user to provide information.

class html_form_input_unit : public body_builder, public events, public actor
{
public:
	// default event name used to signal component change
    static const std::string changed;

    html_form_input_unit(const std::string & label,
			 const libdar::infinint & initial_value,
			 unsigned int size);
    html_form_input_unit(const html_form_input_unit & ref) = default;
    html_form_input_unit(html_form_input_unit && ref) noexcept = default;
    html_form_input_unit & operator = (const html_form_input_unit & ref) = default;
    html_form_input_unit & operator = (html_form_input_unit && ref) noexcept = default;
    ~html_form_input_unit() = default;

	/// set both min and max

	/// \note the condition must be respected 0 <= min < max
    void set_range(const libdar::infinint & x_min, const libdar::infinint & x_max);

	/// set min, min cannot be negative nor less than max if set
    void set_min_only(const libdar::infinint & x_min);

	/// set the max value (by default there is no maximum constraint applied

	/// \note setting this to zero disable max value
    void set_max_only(const libdar::infinint & x_max);

	/// change the label
    void change_label(const std::string & label) { field.change_label(label); };

    void set_value_as_infinint(const libdar::infinint & x_val);
    libdar::infinint get_value_as_infinint() const { return val; };

	/// get the displayed number value (not taking into account the unit)
    const std::string & get_raw_value() const { return field.get_value(); };

	/// get unit display string
    const std::string get_unit_string() const { return unit_box.get_string(); };

	/// whether the HTML control is enable or disabled
    void set_enabled(bool val) { field.set_enabled(val); };

	/// change the name of the event generated when this control changes

	/// \param[in] name must not be an empty string
	/// \note all actors already recorded on the changed event will be kept
	/// registered on this new event name. The old event will no more be triggered
	/// In consequences actor should be aware of the event name change asked to this
	/// component here.
	/// \note this call can be used any number of time, all actors will be kept
    void set_change_event_name(const std::string & name);

	/// get the event name used when the component changes
    std::string get_event_name() const { return modif_change.empty()? changed: modif_change; };

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void css_classes_have_changed() override;

private:
    bool ignore_input_event;
    html_form_input field;
    html_size_unit unit_box;
    libdar::infinint val; ///< current absolute value
    libdar::infinint min; ///< at least zero as we only support positive intergers
    libdar::infinint max; ///< for max, zero means no maximum

    std::string modif_change; ///< name of the event "changed" to use, if set to an empty string default behavior is used

    void my_act() { act(modif_change.empty() ? changed: modif_change); };
    void reduce_val_and_unit();
    void set_field_min_max();
    void set_field_val();


	/// returns the closest value to val expressed in number of "unit"
    static int reduce_to_unit(const libdar::infinint & val, const libdar::infinint & unit);

	/// returns the closest value to val but not less than val expresed in number of "unit"
    static int reduce_to_unit_above(const libdar::infinint & val, const libdar::infinint & unit);

};

#endif
