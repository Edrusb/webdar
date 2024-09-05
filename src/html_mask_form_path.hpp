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

#ifndef HTML_MASK_FORM_PATH_HPP
#define HTML_MASK_FORM_PATH_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_form_mask_bool.hpp"

    /// class html_mask_form_path let user define mask on paths

    /// at the difference of the html_form_* classes which are component to be included into html_form
    /// this class is a full html_form dedicated to the specific case of mask for path filtering

class html_mask_form_path : public html_form
{
public:
    html_mask_form_path(bool allow_absolute_paths);
    html_mask_form_path(const html_mask_form_path & ref) = default;
    html_mask_form_path(html_mask_form_path && ref) noexcept = delete;
    html_mask_form_path & operator = (const html_mask_form_path & ref) = delete;
    html_mask_form_path & operator = (html_mask_form_path && ref) noexcept = delete;
    ~html_mask_form_path() = default;

    void set_fs_root(const std::string & prefix);

	/// inherited from html_mask
    std::unique_ptr<libdar::mask> get_mask() const { return root.get_mask(); };

private:
    html_form_mask_bool root;
};

#endif
