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

#ifndef HTML_FORM_INPUT_HPP
#define HTML_FORM_INPUT_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <vector>

    // webdar headers
#include "body_builder.hpp"
#include "events.hpp"
#include "jsoner.hpp"
#include "html_text.hpp"

    /// class html_form_input implements HTML input feature

    /// it is expected to be adopted by an html_form directory
    /// or through an html_form_fieldset, to provide a mean for
    /// user to provide information.
    /// Depdending on the type set in the constructor argument,
    /// this object is visually a text window, a check box or
    /// let the user select a file on his local directory (local
    /// from browser point of view).

class html_form_input : public body_builder,
			public events,
			public jsoner
{
public:
    static const std::string changed;

    enum input_type { text, password, number, range, check, file };

    html_form_input(const std::string & label,
		    input_type type,
		    const std::string & initial_value,   ///< for check set empty string for unchecked, anything else for checked
		    const std::string & size,            ///< the size in the HTML component is not resizeable use "" to not use it
		    const std::string & css_class);      ///< a CSS class with "max-width/width/min-width" properties *is* reactive and better than size
    html_form_input(const html_form_input & ref) = default;
    html_form_input(html_form_input && ref) noexcept = default;
    html_form_input & operator = (const html_form_input & ref) = default;
    html_form_input & operator = (html_form_input && ref) noexcept = default;
    ~html_form_input() = default;

    void set_range(const libdar::infinint & min, const libdar::infinint & max);
    void set_min_only(const libdar::infinint & min);
    void set_max_only(const libdar::infinint & max);
    void change_label(const std::string & label);
    void change_type(input_type type);

    void set_value(const std::string & val);
    void set_value_as_bool(bool val);
    void set_value_as_int(int val);
    const std::string & get_value() const { return x_init; };
    bool get_value_as_bool() const { return !x_init.empty(); }; //< for checkbox empty string means unchecked, anything else checked
    int get_value_as_int() const;

	/// set whether the HTML control is enable or disabled
    void set_enabled(bool val);

	/// apply css to the box (text box if checkbox, but not to the label)
    void box_set_css_class(const std::string & val) { box_css = val; };

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

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;


private:
    bool enabled;        ///< whether the control is enabled or disabled
    std::string x_label; ///< field text shown to the user
    std::string x_type;  ///< type of HTML input field
    std::string x_init;  ///< current value / initial value of the field
    std::string x_size;  ///< width of the field in number of characterds
    std::string x_min;   ///< minimum value of the field (for type = number)
    std::string x_max;   ///< maximum value of the field (for type = number)
    bool value_set;      ///< record that a manual set_value has been done and POST request received from body_part should not overwrite the change
    std::string box_css; ///< css class applied to the box if non an empty string

    std::string modif_change; ///< name of the event "changed" to use, if set to an empty string default behavior is used

    void my_act() { act(modif_change.empty() ? changed: modif_change); };
    void check_min_max_change(const std::string & next_min, const std::string & next_max);
    std::string generate_label(const std::string & css, const std::string & id);
    std::string generate_input(const std::string & css, const std::string & id);

    html_text new_line;

    static std::string string_for_type(input_type type);

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_form_input";

    static constexpr const char* jlabel_init = "value";

    static const std::string css_label;
    static const std::string css_input;
    static const std::string css_check;
    static const std::string css_checktitle;
    static const std::string css_vertical_space;
    static const std::string css_display_no_CR;
};

#endif
