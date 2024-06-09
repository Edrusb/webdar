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

#ifndef HTML_MASK_FORM_FILENAME_HPP
#define HTML_MASK_FORM_FILENAME_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_form_mask_bool.hpp"
#include "html_form_mask_expression.hpp"
#include "html_form.hpp"

    /// class html_mask_form_filename let user define mask on filenames

class html_mask_form_filename : public html_form
{
public:
    html_mask_form_filename();
    html_mask_form_filename(const html_mask_form_filename & ref) = default;
    html_mask_form_filename(html_mask_form_filename && ref) noexcept = delete;
    html_mask_form_filename & operator = (const html_mask_form_filename & ref) = delete;
    html_mask_form_filename & operator = (html_mask_form_filename && ref) noexcept = delete;
    ~html_mask_form_filename() = default;

	/// inherited from html_mask
    std::unique_ptr<libdar::mask> get_mask() const { return root.get_mask(); };

private:
    html_form_mask_bool root;
};

#endif
