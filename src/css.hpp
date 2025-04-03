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

#ifndef CSS_HPP
#define CSS_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>
#include <deque>
#include <map>

    // webdar headers
#include "css_property.hpp"

    /// class managing Cascading Style Sheets attributes

    /// contains a set of well-known css attributes and their values
    /// provide methods to reset each of them to a well-known default value

class css
{
public:

    css() { clear(); };
    css(const css & ref) = default;
    css(css && ref) noexcept = default;
    css & operator = (const css & ref) = default;
    css & operator = (css && ref) noexcept = default;
    virtual ~css() = default;

	/// update current object with argument ignoring all fields which are unset
    void update_from(const css & ref);

	/// set css attributes to their default
    void clear();

	// colors

    void css_color(const std::string & col);
    void css_background_color(const std::string & col);
    void css_background_color() { bg_col.clear(); }; // unset previously set value

	// background image

    void css_background_image(const std::string & url,
			      bool repeat_x,
			      bool repeat_y,
			      bool fixed);
    void css_background_image() { bg_img.clear(); };

    void css_background_position(const std::string & x,
				 const std::string & y);
    void css_background_position() { img_pos.clear(); };

	// shadow
    void css_box_shadow() { box_shadow.clear(); };
    void css_box_shadow(const std::string & x_shift,
			const std::string & y_shift,
			const std::string & blur_size,
			const std::string & color = "#444444");

    void css_text_shadow() { text_shadow.clear(); };
    void css_text_shadow(const std::string & x_shift,
			 const std::string & y_shift,
			 const std::string & blur_size,
			 const std::string & color = "#444444");

	// position in the window

    void css_margin(const std::string & all);
    void css_margin() { css_margin_top(); css_margin_right(); css_margin_bottom(); css_margin_left(); };
    void css_margin_top(const std::string & top);
    void css_margin_top() { margin_top.clear(); };
    void css_margin_right(const std::string & right);
    void css_margin_right() { margin_right.clear(); };
    void css_margin_bottom(const std::string & bottom);
    void css_margin_bottom() { margin_bottom.clear(); };
    void css_margin_left(const std::string & left);
    void css_margin_left() { margin_left.clear(); };
    void css_height(const std::string & val,
		    bool center);
    void css_height() { height.clear(); };
    void css_min_height(const std::string & val);
    void css_min_height() { min_height.clear(); };
    void css_max_height(const std::string & val);
    void css_max_height() { max_height.clear(); };
    void css_width(const std::string & val,
		   bool center);
    void css_width() { width.clear(); };
    void css_min_width(const std::string & val);
    void css_min_width() { min_width.clear(); };
    void css_max_width(const std::string & val);
    void css_max_width() { max_width.clear(); };
    void css_z_index(unsigned int index);
    void css_z_index() { z_index.clear(); };

    enum positionning { pos_absolute,  ///< absolute in the document
			pos_relative,  ///< shifted relative to its expected position
			pos_fixed,     ///< fixed in the viewport
			pos_sticky     ///< either relative or fixed depending on scrolling of parent object
    };

    void css_position_type(positionning val);
    void css_position_type() { position_type.clear(); };
    void css_position_top(const std::string & top);
    void css_position_top() { position_top.clear(); };
    void css_position_left(const std::string & left);
    void css_position_left() { position_left.clear(); };
    void css_position_bottom(const std::string & bottom);
    void css_position_bottom() { position_bottom.clear(); };
    void css_position_right(const std::string & right);
    void css_position_right() { position_right.clear(); };

    enum overflowing { ov_visible,   // show even what exceed the parent dimensions
		       ov_hidden,    //< do not show what exceeds the parent dimensions
		       ov_scroll,    //< alsways add a scrollbar to see what's hidden
		       ov_auto,      //< scrollbar shows only if necessary
		       ov_inherit }; //< inherited from HTML parent

	/// set both x and y overflow

	/// \note css_overflow, css_overflow_x and css_overflow_y are mutual exclusive
    void css_overflow(overflowing val);

	/// set only x overflow
    void css_overflow_x(overflowing val);

	/// set only y overflow
    void css_overflow_y(overflowing val);

	/// clear both x and y overflow (default value)
    void css_overflow() { overflow.clear(); };

	// floating

    enum floating { fl_left,
		    fl_right,
		    fl_none };

    enum floatclear { fc_left,
		      fc_right,
		      fc_both,
		      fc_none };

    void css_float(floating val);
    void css_float() { float_pos.clear(); };
    void css_float_clear(floatclear val);
    void css_float_clear() { float_clear.clear(); };

	// opacity

    void css_opacity(const std::string & opacity);


	// padding

    enum bx_sizing
    {
	bx_content, ///< width and height define the content (add padding and border size for the real size)
	bx_border   ///< width and height define the box size (reduce content space to fit within the border+padding)
    };

    void css_box_sizing(bx_sizing val);

    void css_display(const std::string & val);

    void css_padding(const std::string & val);
    void css_padding() { css_padding_top(); css_padding_right(); css_padding_bottom(); css_padding_left(); };
    void css_padding_top(const std::string & top);
    void css_padding_top() { padding_top.clear(); };
    void css_padding_right(const std::string & right);
    void css_padding_right() { padding_right.clear(); };
    void css_padding_bottom(const std::string & bottom);
    void css_padding_bottom() { padding_bottom.clear(); };
    void css_padding_left(const std::string & left);
    void css_padding_left() { padding_left.clear(); };

	// text

    enum h_align
    {
	al_right,
	al_left,
	al_center,
	al_justify
    };

    enum v_align
    {
	al_top,
	al_middle,
	al_bottom,
	al_baseline,
	al_sub,
	al_super,
	al_text_top,
	al_text_bottom
    };

    enum decoration
    {
	dc_underline,
	dc_overline,
	dc_line_through,
	dc_none
    };

    void css_font_size(const std::string & val);
    void css_font_style_italic();
    void css_font_style_normal();
    void css_font_style() { font_style.clear(); };
    void css_font_weight_bold();
    void css_font_weight_normal();
    void css_font_weight() { font_weight.clear(); };
    void css_text_h_align(h_align val);
    void css_text_h_align() { text_h_align.clear(); };
    void css_text_v_align(v_align val);
    void css_text_v_align() { text_v_align.clear(); };
    void css_text_decoration(decoration val);
    void css_text_decoration() { text_deco.clear(); };

	// border

    enum border { bd_top,
		  bd_right,
		  bd_bottom,
		  bd_left,
		  bd_all,
		  bd_clear }; //< remove specifications for any border

    enum bd_width { bd_thin,
		    bd_medium,
		    bd_thick };

    enum bd_style { bd_dotted,
		    bd_dashed,
		    bd_solid,
		    bd_double,
		    bd_groove,
		    bd_ridge,
		    bd_inset,
		    bd_outset,
		    bd_none };

    void css_border_width(border which, bd_width val);
    void css_border_width() { border_width.clear(); };
    void css_border_color(border which, const std::string & col);
    void css_border_color() { border_color.clear(); };
    void css_border_style(border which, bd_style val);
    void css_border_style() { border_style.clear(); };

	// round corners
    void css_corner_radius(const std::string & all);
    void css_corner_radius(const std::string & topleft,
			   const std::string & topright,
			   const std::string & botright,
			   const std::string & botleft);

	// clear all css_corner radius relative information
    void css_corner_radius() { corner_radius.clear(); };

	/// css_visibility does not prevent sending HTML code to the browser

	/// \note body_builder class has a set_visible() method that avoids a
	/// component to produce html code though it keeps it place in the adoption tree
	/// but no html code is sent to the browser. Here CSS visibility sends
	/// html code to the browser and may lead an object to change its visibility without
	/// webdar - browser communication (Responsive CSS)
    void css_visibility(bool val);

	/// grid feature

	/// parent html object defines a grid-template-areas

	/// \note this does not modify the existing css grid added lines
    void css_grid_template(const std::string & gap);

	/// remove the grid template parameters and lines (not the grid area ones)
    void css_grid_template() { grid_template.clear(); grid_template_lines.clear(); };

	/// a grid-template-areas defines lines containing space separated area names

	/// \param[in] line is expected to be a space separated list of area names
	/// \note all line should have the same number of areas names (making colums accross lines)
	/// \note if a name area spaws more than one column it has to be repeated
    void css_grid_template_add_line(const std::string & line);

	/// clear all lines of a css_grid_template
    void css_grid_template_clear_lines() { grid_template_lines.clear(); };

	/// components adopted by a body_builder having defined a grid_template are assigned area names

	/// \note only one area name can be assigned to a given component
    void css_grid_area_name(const std::string & name);

	/// clears the grid area name property
    void css_grid_area_name() { grid_area.clear(); };


	/// returns the css raw string
    std::string css_get_raw_string() const;

protected:

	/// this method is called when a property has been modified
    virtual void css_updated() {};

    	/// custom css properties, method available for inherited classes

	/// \note storing css properties by this css methods
	/// lead them to be integrated in the css_get_string/css_get_raw_string methods
	/// they also can be abstracted to html class by body builder derived class

	/// note, that this provided label must not have already been declared previously on that css
    void declare_custom_css(const std::string & label);

	/// set value to a previously declared custom css
    void set_custom_css(const std::string & label, const std::string & val);

	/// unset value of a previously declared custom css
    void clear_custom_css(const std::string & label);


private:

	// colors properties
    css_property color;
    css_property bg_col;

	// background image properties
    css_property bg_img;
    css_property img_pos;

	// shadowing
    css_property box_shadow;
    css_property text_shadow;

	// position properties
    css_property box_sizing;
    css_property display;
    css_property margin_top;
    css_property margin_right;
    css_property margin_bottom;
    css_property margin_left;
    css_property height;
    css_property min_height;
    css_property max_height;
    css_property width;
    css_property min_width;
    css_property max_width;
    css_property z_index;
    css_property position_type;
    css_property position_top;
    css_property position_left;
    css_property position_bottom;
    css_property position_right;
    css_property overflow;

	// floating properties
    css_property float_pos;
    css_property float_clear;
    css_property opacity;

	// padding properties
    css_property padding_top;
    css_property padding_right;
    css_property padding_bottom;
    css_property padding_left;

	// text properties
    css_property font_size;
    css_property font_style;
    css_property font_weight;
    css_property text_h_align;
    css_property text_v_align;
    css_property text_deco;

	// border properties
    css_property border_width;
    css_property border_color;
    css_property border_style;

	// round corners
    css_property corner_radius;

	// CSS visibility
    css_property visibility;

	// CSS grid
    css_property grid_template;
    css_property grid_template_lines;
    css_property grid_area;

	// custom css
    std::map<std::string, css_property> custom_css;

    std::string border_to_string(border val);
    std::string overflow_to_string(overflowing val);
};

#endif
