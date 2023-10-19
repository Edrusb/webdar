/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
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
extern "C"
{

}

    // C++ system header files
#include <string>
#include <deque>
#include <map>

    // webdar headers
#include "css_property.hpp"

    /// class css
    ///
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

	/// set css attributes to their default
    void clear();

	// colors

    void css_color(const std::string & col,
		   bool inherit=false);
    void css_background_color(const std::string & col,
			      bool inherit=false);
    void css_background_color() { bg_col.clear(); }; // unset previously set value

	// background image

    void css_background_image(const std::string & url,
			      bool repeat_x,
			      bool repeat_y,
			      bool fixed,
			      bool inherit=false);
    void css_background_image() { bg_img.clear(); };

    void css_background_position(const std::string & x,
				     const std::string & y,
				     bool inherit=false);
    void css_background_position() { img_pos.clear(); };

	// position in the window

    void css_margin(const std::string & all,
		    bool inherit=false);
    void css_margin() { css_margin_top(); css_margin_right(); css_margin_bottom(); css_margin_left(); };
    void css_margin_top(const std::string & top,
			bool inherit=false);
    void css_margin_top() { margin_top.clear(); };
    void css_margin_right(const std::string & right,
			  bool inherit=false);
    void css_margin_right() { margin_right.clear(); };
    void css_margin_bottom(const std::string & bottom,
			   bool inherit=false);
    void css_margin_bottom() { margin_bottom.clear(); };
    void css_margin_left(const std::string & left,
			 bool inherit=false);
    void css_margin_left() { margin_left.clear(); };
    void css_height(const std::string & val,
		    bool center,
		    bool inherit=false);
    void css_height() { height.clear(); };
    void css_width(const std::string & val,
		   bool center,
		   bool inherit=false);
    void css_width() { width.clear(); };
    void css_z_index(unsigned int index,
		     bool inherit=false);
    void css_z_index() { z_index.clear(); };

    enum positionning { pos_absolute,  //< absolute in the document
			pos_relative,  //< shifted relative to its expected position
			pos_fixed };  //< fixed in the viewport

    void css_position_type(positionning val,
			   bool inherit=false);
    void css_position_type() { position_type.clear(); };
    void css_position_top(const std::string & top,
			  bool inherit=false);
    void css_position_top() { position_top.clear(); };
    void css_position_left(const std::string & left,
			   bool inherit=false);
    void css_position_left() { position_left.clear(); };
    void css_position_bottom(const std::string & bottom,
			     bool inherit=false);
    void css_position_bottom() { position_bottom.clear(); };
    void css_position_right(const std::string & right,
			    bool inherit=false);
    void css_position_right() { position_right.clear(); };

    enum overflowing { ov_visible,   // show even what exceed the parent dimensions
		       ov_hidden,    //< do not show what exceeds the parent dimensions
		       ov_scroll,    //< alsways add a scrollbar to see what's hidden
		       ov_auto,      //< scrollbar shows only if necessary
		       ov_inherit }; //< inherited from HTML parent

    void css_overflow(overflowing val, bool inherit=false);
    void css_overflow() { overflow.clear(); };

	// floating

    enum floating { fl_left,
		    fl_right,
		    fl_none };

    enum floatclear { fc_left,
		      fc_right,
		      fc_both,
		      fc_none };

    void css_float(floating val, bool inherit=false);
    void css_float() { float_pos.clear(); };
    void css_float_clear(floatclear val, bool inherit=false);
    void css_float_clear() { float_clear.clear(); };

	// padding

    void css_padding(const std::string & val,
		     bool inherit=false);
    void css_padding() { css_padding_top(); css_padding_right(); css_padding_bottom(); css_padding_left(); };
    void css_padding_top(const std::string & top,
			 bool inherit=false);
    void css_padding_top() { padding_top.clear(); };
    void css_padding_right(const std::string & right,
			   bool inherit=false);
    void css_padding_right() { padding_right.clear(); };
    void css_padding_bottom(const std::string & bottom,
			    bool inherit=false);
    void css_padding_bottom() { padding_bottom.clear(); };
    void css_padding_left(const std::string & left,
			  bool inherit=false);
    void css_padding_left() { padding_left.clear(); };

	// text

    enum align { al_right,
		 al_left,
		 al_center,
		 al_justify };

    enum decoration { dc_underline,
		      dc_overline,
		      dc_line_through,
		      dc_none };

    void css_font_style_italic(bool inherit=false);
    void css_font_style_normal(bool inherit=false);
    void css_font_style() { font_style.clear(); };
    void css_font_weight_bold(bool inherit=false);
    void css_font_weight_normal(bool inherit=false);
    void css_font_weight() { font_weight.clear(); };
    void css_text_align(align val, bool inherit=false);
    void css_text_align() { text_align.clear(); };
    void css_text_decoration(decoration val, bool inherit=false);
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

    void css_border_width(border which, bd_width val, bool inherit=false);
    void css_border_width() { border_width.clear(); };
    void css_border_color(border which, const std::string & col, bool inherit=false);
    void css_border_color() { border_color.clear(); };
    void css_border_style(border which, bd_style val, bool inherit=false);
    void css_border_style() { border_style.clear(); };

	// round corners
    void css_corner_radius(const std::string & all, bool inherit=false);
    void css_corner_radius(const std::string & topleft,
			   const std::string & topright,
			   const std::string & botright,
			   const std::string & botleft,
			   bool inherit=false);
    void css_corner_radius(); // clear all css_corner radius relative information

	/// css_visibility does not prevent sending HTML code to the browser

	/// \note body_builder class has a set_visible() method that avoids a
	/// component to produce html code though it keeps it place in the adoption tree
	/// but no html code is sent to the browser. Here CSS visibility sends
	/// html code to the browser and may lead an object to change its visibility without
	/// webdar - browser communication (Responsive CSSS)
    void css_visibility(bool val, bool inherit=false);


	/// inherit css properties and html classes from the given reference
	///
	/// \param[in] ref from which to inherit properties
	/// \param[in] all if true even properties of "ref" having inheritance unset are taken into account
	/// \param[in] force if false, only unset properties of "this" are inhertied, we do not erase existing properties by those of "ref"
	/// \note when both "all" and "force" are false only unset properties of "this" are set to the value of the given reference
	/// and only inheritable properties of the references are copied
    void css_inherit_from(const css & ref, bool all=false, bool force=false);

	/// returns the css raw string
    std::string css_get_raw_string() const;

protected:

	// this method is called when a property has been modified
	//
	// \param[in] inherit is set to true if the modified property has inheritance set
    virtual void css_updated(bool inherited) {};

    	/// custom css properties, method available for inherited classes

	/// \note storing css properties par this css parent class
	/// lead them to be integrated in the css_get_string/css_get_raw_string methods
	/// they also can be abstracted to html class by body builder derived class

	/// note, that this provided label must not have already been declared previously on that css
    void declare_custom_css(const std::string & label);

	/// set value to a previously declared custom css
    void set_custom_css(const std::string & label, const std::string & val, bool inherit=false);

	/// unset value of a previously declared custom css
    void clear_custom_css(const std::string & label);


private:

	// colors properties
    css_property color;
    css_property bg_col;

	// background image properties
    css_property bg_img;
    css_property img_pos;

	// position properties
    css_property margin_top;
    css_property margin_right;
    css_property margin_bottom;
    css_property margin_left;
    css_property height;
    css_property width;
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

	// padding properties
    css_property padding_top;
    css_property padding_right;
    css_property padding_bottom;
    css_property padding_left;

	// text properties
    css_property font_style;
    css_property font_weight;
    css_property text_align;
    css_property text_deco;

	// border properties
    css_property border_width;
    css_property border_color;
    css_property border_style;

	// round corners
    css_property corner_radius;

	// CSS visibility
    css_property visibility;

	// custom css
    std::map<std::string, css_property> custom_css;

    std::string border_to_string(border val);
};

#endif
