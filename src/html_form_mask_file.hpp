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

#ifndef HTML_FORM_MASK_FILE_HPP
#define HTML_FORM_MASK_FILE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_mask.hpp"
#include "html_form_input.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_input_file.hpp"
#include "actor.hpp"
#include "jsoner.hpp"

    /// class html_form_mask_file provide interface to include or exclude a list of entry taken from a text file

    /// this html components is consitued of:
    ///

    /** \verbatim

	+----html_fieldset----------------------------------+
	|                                                   |
	|          +-----------------------------+          |
	| filename |                             |          |
	|          +-----------------------------+          |
	|                                                   |
	| exclude checkbox : +-+                            |
	|                    | |                            |
	|                    +-+                            |
	| casesensit       : +-+                            |
	|                    | |                            |
	|                    +-+                            |
	|                                                   |
	+---------------------------------------------------+

    \endverbatim **/

    /// \note this component includes an html_form by default
    /// (see constructor argument), but should not when used
    /// recursively from another html_form_mask_file


class html_form_mask_file : public html_mask,
			    public actor,
			    public jsoner
{
public:
    html_form_mask_file(const std::shared_ptr<const libdar::path> & fs_root);
    html_form_mask_file(const html_form_mask_file & ref);
    html_form_mask_file(html_form_mask_file && ref) noexcept = delete;
    html_form_mask_file & operator = (const html_form_mask_file & ref) = delete;
    html_form_mask_file & operator = (html_form_mask_file && ref) noexcept = delete;
    ~html_form_mask_file() = default;

	/// reset object to default
    void clear();

	/// inherited from html_mask
    virtual std::unique_ptr<libdar::mask> get_mask() const override;

        /// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override { clear(); };


	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;


protected:
	/// inherited methods from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    html_form_input_file filename;
    html_form_input exclude_checkbox;
    html_form_input casesensit;
    html_form_fieldset fs;
    std::shared_ptr<const libdar::path> prefix;

    void init();
    std::string tell_action() const;

    static constexpr const unsigned int format_version = 1;

    static constexpr const char* jlabel_filename = "filename";
    static constexpr const char* jlabel_exclude = "excluding";
    static constexpr const char* jlabel_casesensit = "casesensit";

};

#endif
