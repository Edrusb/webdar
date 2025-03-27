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

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "exceptions.hpp"
#include "webdar_tools.hpp"


    //
#include "css.hpp"

using namespace std;



void css::clear()
{
    color.clear();
    bg_col.clear();
    bg_img.clear();
    img_pos.clear();
    box_shadow.clear();
    text_shadow.clear();
    box_sizing.clear();
    display.clear();
    margin_top.clear();
    margin_right.clear();
    margin_bottom.clear();
    margin_left.clear();
    height.clear();
    width.clear();
    max_width.clear();
    z_index.clear();
    position_type.clear();
    position_top.clear();
    position_left.clear();
    position_bottom.clear();
    position_right.clear();
    overflow.clear();
    float_pos.clear();
    float_clear.clear();
    opacity.clear();
    padding_top.clear();
    padding_right.clear();
    padding_bottom.clear();
    padding_left.clear();
    font_size.clear();
    font_style.clear();
    font_weight.clear();
    text_h_align.clear();
    text_v_align.clear();
    text_deco.clear();
    border_width.clear();
    border_color.clear();
    border_style.clear();
    corner_radius.clear();

    map<string, css_property>::iterator it = custom_css.begin();
    while(it != custom_css.end())
    {
	it->second.clear();
	++it;
    }
}

void css::css_color(const string & col,
		    bool inherit)
{
    color.set_value(string(" color: ") + col + ";");
    color.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_background_color(const string & col,
			       bool inherit)
{
    bg_col.set_value(string(" background-color: ") + col + ";");
    bg_col.set_inheritance(inherit);
    css_updated(inherit);
}


void css::css_background_image(const string & url,
			       bool repeat_x,
			       bool repeat_y,
			       bool fixed,
			       bool inherit)
{
    string tmp = " background-repeat: ";
    string val = string(" background-image: url(\"") + url + "\");";
    if(repeat_x)
	if(repeat_y)
	    val += tmp + "repeat;";
	else
	    val += tmp + "repeat-x;";
    else
	if(repeat_y)
	    val += tmp + "repeat-y;";
	else
	    val += tmp + "no-repeat;";
    val += string(" background-attachment: ") + (fixed ? "fixed;" : "scroll;");

    bg_img.set_value(val);
    bg_img.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_background_position(const string & x,
				  const string & y,
				  bool inherit)
{
    if(x != "" && y != "") // yes both
    {
	img_pos.set_value(string(" background-position: ") + x + " " + y + ";");
	img_pos.set_inheritance(inherit);
    }
    else
	img_pos.clear();
    css_updated(inherit);
}

void css::css_box_shadow(const string & x_shift,
			 const string & y_shift,
			 const string & blur_size,
			 const string & color,
			 bool inherit)
{
    box_shadow.set_value(string(" box-shadow: ")
			 + x_shift + " "
			 + y_shift + " "
			 + blur_size + " "
			 + color + ";");
    box_shadow.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_text_shadow(const string & x_shift,
			  const string & y_shift,
			  const string & blur_size,
			  const string & color,
			  bool inherit)
{
    text_shadow.set_value(string(" text-shadow: ")
			  + x_shift + " "
			  + y_shift + " "
			  + blur_size + " "
			  + color + ";");
    text_shadow.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_box_sizing(bx_sizing val,
			 bool inherit)
{
    string tmp;

    switch(val)
    {
    case bx_content:
	tmp = "content-box";
	break;
    case bx_border:
	tmp = "border-box";
	break;
    default:
	throw WEBDAR_BUG;
    }

    box_sizing.set_value(string(" box-sizing: ") + tmp + ";");
    box_sizing.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_display(const string & val, bool inherit)
{
    display.set_value(string(" display: ") + val + ";");
    display.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_margin(const string & all, bool inherit)
{
    css_margin_top(all, inherit);
    css_margin_right(all, inherit);
    css_margin_bottom(all, inherit);
    css_margin_left(all, inherit);
}

void css::css_margin_top(const string & top, bool inherit)
{
    margin_top.set_value(string(" margin-top: ") + top + ";");
    margin_top.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_margin_right(const string & right, bool inherit)
{
    margin_right.set_value(string(" margin-right: ") + right + ";");
    margin_right.set_inheritance(inherit);
    css_updated(inherit);

}
void css::css_margin_bottom(const string & bottom, bool inherit)
{
    margin_bottom.set_value(string(" margin-bottom: ") + bottom + ";");
    margin_bottom.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_margin_left(const string & left, bool inherit)
{
    margin_left.set_value(string(" margin-left: ") + left + ";");
    margin_left.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_height(const string & val,
		     bool center,
		     bool inherit)
{
    height.set_value(string(" height: ") + val + ";");
    height.set_inheritance(inherit);
    if(center)
    {
	css_margin_top("auto", inherit);
	css_margin_bottom("auto", inherit);
    }
    css_updated(inherit);
}

void css::css_width(const string & val,
		    bool center,
		    bool inherit)
{
    width.set_value(string(" width: ") + val + ";");
    width.set_inheritance(inherit);
    if(center)
    {
	css_margin_left("auto", inherit);
	css_margin_right("auto", inherit);
    }
    css_updated(inherit);
}

void css::css_max_width(const string & val,
			bool inherit)
{
    max_width.set_value(string(" max-width: ") + val + ";");
    width.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_z_index(unsigned int index, bool inherit)
{
    z_index.set_value(string(" z-index: ") + webdar_tools_convert_to_string(index) + ";");
    z_index.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_position_type(positionning val,
			    bool inherit)
{
    static const string command = " position: ";

    switch(val)
    {
    case pos_absolute:
	position_type.set_value(command + "absolute;");
	break;
    case pos_relative:
	position_type.set_value(command + "relative;");
	break;
    case pos_fixed:
	position_type.set_value(command + "fixed;");
	break;
    case pos_sticky:
	position_type.set_value(command + "sticky;" + command + "-webkit-sticky;");
	break;
    default:
	throw WEBDAR_BUG;
    }


    position_type.set_inheritance(inherit);
    css_updated(inherit);
}


void css::css_position_top(const string & top,
			  bool inherit)
{
    position_top.set_value(string(" top:") + top + ";");
    position_top.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_position_left(const string & left,
			   bool inherit)
{
    position_left.set_value(string(" left:") + left + ";");
    position_left.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_position_bottom(const string & bottom,
			     bool inherit)
{
    position_bottom.set_value(string(" bottom:") + bottom + ";");
    position_bottom.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_position_right(const string & right,
			    bool inherit)
{
    position_right.set_value(string(" right:") + right + ";");
    position_right.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_overflow(overflowing val, bool inherit)
{
    string arg = " overflow: " + overflow_to_string(val);

    overflow.set_value(arg);
    overflow.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_overflow_x(overflowing val, bool inherit)
{
    string arg = " overflow-x: " + overflow_to_string(val);

    overflow.set_value(arg);
    overflow.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_overflow_y(overflowing val, bool inherit)
{
    string arg = " overflow-y: " + overflow_to_string(val);

    overflow.set_value(arg);
    overflow.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_float(floating val, bool inherit)
{
    string arg = " float: ";

    switch(val)
    {
    case fl_left:
	arg += "left;";
	break;
    case fl_right:
	arg += "right;";
	break;
    case fl_none:
	arg += "none;";
	break;
    default:
	throw WEBDAR_BUG;
    }

    float_pos.set_value(arg);
    float_pos.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_float_clear(floatclear val, bool inherit)
{
    string arg = " clear: ";

    switch(val)
    {
    case fc_left:
	arg += "left;";
	break;
    case fc_right:
	arg += "right;";
	break;
    case fc_both:
	arg += "both;";
	break;
    case fc_none:
	arg += "none;";
	break;
    default:
	throw WEBDAR_BUG;
    }
    float_clear.set_value(arg);
    float_clear.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_opacity(const string & val,
		      bool inherit)
{
    opacity.set_value(string(" opacity: ") + val + ";");
    opacity.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_padding(const string & val,
		      bool inherit)
{
    css_padding_top(val, inherit);
    css_padding_right(val, inherit);
    css_padding_bottom(val, inherit);
    css_padding_left(val, inherit);
}

void css::css_padding_top(const string & top,
		     bool inherit)
{
    padding_top.set_value(string(" padding-top: ") + top + ";");
    padding_top.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_padding_right(const string & right,
		       bool inherit)
{
    padding_right.set_value(string(" padding-right: ") + right + ";");
    padding_right.set_inheritance(inherit);
    css_updated(inherit);
}
void css::css_padding_bottom(const string & bottom,
			bool inherit)
{
    padding_bottom.set_value(string(" padding-bottom: ") + bottom + ";");
    padding_bottom.set_inheritance(inherit);
    css_updated(inherit);
}
void css::css_padding_left(const string & left,
		      bool inherit)
{
    padding_left.set_value(string(" padding-left: ") + left + ";");
    padding_left.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_font_size(const std::string & val,
			bool inherit)
{
    font_size.set_value(string(" font-size: ") + val + ";");
    font_size.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_font_style_italic(bool inherit)
{
    font_style.set_value(" font-style: italic;");
    font_style.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_font_style_normal(bool inherit)
{
    font_style.set_value(" font-style: normal;");
    font_style.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_font_weight_bold(bool inherit)
{
    font_weight.set_value(" font-weight: bold;");
    font_weight.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_font_weight_normal(bool inherit)
{
    font_weight.set_value(" font-weight: normal;");
    font_weight.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_text_h_align(h_align val, bool inherit)
{
    string arg = " text-align: ";
    switch(val)
    {
    case al_right:
	arg += "right;";
	break;
    case al_left:
	arg += "left;";
	break;
    case al_center:
	arg += "center;";
	break;
    case al_justify:
	arg += "justify;";
	break;
    default:
	throw WEBDAR_BUG;
    }
    text_h_align.set_value(arg);
    text_h_align.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_text_v_align(v_align val, bool inherit)
{
    string arg = " vertical-align: ";
    switch(val)
    {
    case al_top:
	arg += "top;";
	break;
    case al_middle:
	arg += "middle;";
	break;
    case al_bottom:
	arg += "bottom;";
	break;
    case al_baseline:
	arg += "baseline;";
	break;
    case al_sub:
	arg += "sub;";
	break;
    case al_super:
	arg += "super;";
	break;
    case al_text_top:
	arg += "text-top;";
	break;
    case al_text_bottom:
	arg += "text-bottom;";
	break;
    default:
	throw WEBDAR_BUG;
    }
    text_v_align.set_value(arg);
    text_v_align.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_text_decoration(decoration val, bool inherit)
{
    string arg = " text-decoration: ";

    switch(val)
    {
    case dc_underline:
	arg += "underline;";
	break;
    case dc_overline:
	arg += "overline;";
	break;
    case dc_line_through:
	arg += "line-through;";
	break;
    case dc_none:
	arg += "none;";
	break;
    default:
	throw WEBDAR_BUG;
    }
    text_deco.set_value(arg);
    text_deco.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_border_width(border which, bd_width val, bool inherit)
{
    string arg = string(" border") + border_to_string(which) + "width: ";

    switch(val)
    {
    case bd_thin:
	arg += "thin;";
	break;
    case bd_medium:
	arg += "medium;";
	break;
    case bd_thick:
	arg += "thick;";
	break;
    default:
	throw WEBDAR_BUG;
    }

    border_width.set_inheritance(inherit);
    switch(which)
    {
    case bd_all:
	border_width.set_value(arg);
	break;
    case bd_clear:
	border_width.clear();
	break;
    default:
	border_width.set_value(border_width.get_value() + arg);
	break; // not usefull here, but does not hurt
    }
    css_updated(inherit);
}

void css::css_border_color(border which, const string & col, bool inherit)
{
    string arg = string(" border") + border_to_string(which) + "color: ";

    arg += col + ";";

    border_color.set_inheritance(inherit);
    switch(which)
    {
    case bd_all:
	border_color.set_value(arg);
	break;
    case bd_clear:
	border_color.clear();
	break;
    default:
	border_color.set_value(border_color.get_value() + arg);
	break; // not usefull here, but does not hurt
    }
    css_updated(inherit);
}

void css::css_border_style(border which, bd_style val, bool inherit)
{
    string arg = string(" border") + border_to_string(which) + "style: ";

    switch(val)
    {
    case bd_dotted:
	arg += "dotted;";
	break;
    case bd_dashed:
	arg += "dashed;";
	break;
    case bd_solid:
	arg += "solid;";
	break;
    case bd_double:
	arg += "double;";
	break;
    case bd_groove:
	arg += "groove;";
	break;
    case bd_ridge:
	arg += "ridge;";
	break;
    case bd_inset:
	arg += "inset;";
	break;
    case bd_outset:
	arg += "outset;";
	break;
    case bd_none:
	arg += "none;";
	break;
    default:
	throw WEBDAR_BUG;
    }

    border_style.set_inheritance(inherit);
    switch(which)
    {
    case bd_all:
	border_style.set_value(arg);
	break;
    case bd_clear:
	border_style.clear();
	break;
    default:
	border_style.set_value(border_style.get_value() + arg);
	break; // not usefull here, but does not hurt
    }
    css_updated(inherit);
}

void css::css_corner_radius(const string & all, bool inherit)
{
    css_corner_radius(all, all, all, all, inherit);
}


void css::css_corner_radius(const string & topleft,
			    const string & topright,
			    const string & botright,
			    const string & botleft,
			    bool inherit)
{
    string arg = " border-radius: ";

    if(topleft == topright
       && botleft == botright
       && topleft == botleft) // all are equal
	arg += topleft + ";";
    else
	arg += topleft + " " + topright + " " + botright + " " + botleft + ";";

    corner_radius.set_value(arg);
    corner_radius.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_visibility(bool val, bool inherit)
{
    string text_val = val ? "visible" : "hidden";

    visibility.set_value(string(" visibility: ") + text_val + ";");
    visibility.set_inheritance(inherit);
    css_updated(inherit);
}


void css::css_inherit_from(const css & ref, bool all, bool force)
{
    color.inherit_from(ref.color, all, force);
    bg_col.inherit_from(ref.bg_col, all, force);
    bg_img.inherit_from(ref.bg_img, all, force);
    img_pos.inherit_from(ref.img_pos, all, force);
    box_shadow.inherit_from(ref.box_shadow, all, force);
    text_shadow.inherit_from(ref.text_shadow, all, force);
    box_sizing.inherit_from(ref.box_sizing, all, force);
    display.inherit_from(ref.display, all, force);
    margin_top.inherit_from(ref.margin_top, all, force);
    margin_right.inherit_from(ref.margin_right, all, force);
    margin_bottom.inherit_from(ref.margin_bottom, all, force);
    margin_left.inherit_from(ref.margin_left, all, force);
    height.inherit_from(ref.height, all, force);
    width.inherit_from(ref.width, all, force);
    max_width.inherit_from(ref.max_width, all, force);
    z_index.inherit_from(ref.z_index, all, force);
    position_type.inherit_from(ref.position_type, all, force);
    position_top.inherit_from(ref.position_top, all, force);
    position_left.inherit_from(ref.position_left, all, force);
    position_bottom.inherit_from(ref.position_bottom, all, force);
    position_right.inherit_from(ref.position_right, all, force);
    overflow.inherit_from(ref.overflow, all, force);
    float_pos.inherit_from(ref.float_pos, all, force);
    float_clear.inherit_from(ref.float_clear, all, force);
    opacity.inherit_from(ref.opacity, all, force);
    padding_top.inherit_from(ref.padding_top, all, force);
    padding_right.inherit_from(ref.padding_right, all, force);
    padding_bottom.inherit_from(ref.padding_bottom, all, force);
    padding_left.inherit_from(ref.padding_left, all, force);
    font_size.inherit_from(ref.font_size, all, force);
    font_style.inherit_from(ref.font_style, all, force);
    font_weight.inherit_from(ref.font_weight, all, force);
    text_h_align.inherit_from(ref.text_h_align, all, force);
    text_v_align.inherit_from(ref.text_v_align, all, force);
    text_deco.inherit_from(ref.text_deco, all, force);
    border_width.inherit_from(ref.border_width, all, force);
    border_color.inherit_from(ref.border_color, all, force);
    border_style.inherit_from(ref.border_style, all, force);
    corner_radius.inherit_from(ref.corner_radius, all, force);
    visibility.inherit_from(ref.visibility, all, force);

	// custom css inheritance

    map<string, css_property>::const_iterator crefit = ref.custom_css.begin();
    map<string, css_property>::iterator cit;

    while(crefit != ref.custom_css.end())
    {
	cit = custom_css.find(crefit->first);
	if(cit != custom_css.end())
	    cit->second.inherit_from(crefit->second, all, force);
	else
	{
	    if(crefit->second.get_inheritance() || all)
		custom_css[crefit->first] = crefit->second;
	}
	++crefit;
    }

    css_updated(true);
}

string css::css_get_raw_string() const
{
    string ret = "";

    ret += color.get_value();
    ret += bg_col.get_value();
    ret += bg_img.get_value();
    ret += img_pos.get_value();
    ret += box_shadow.get_value();
    ret += text_shadow.get_value();
    ret += box_sizing.get_value();
    ret += display.get_value();
    ret += margin_top.get_value();
    ret += margin_right.get_value();
    ret += margin_bottom.get_value();
    ret += margin_left.get_value();
    ret += height.get_value();
    ret += width.get_value();
    ret += max_width.get_value();

    if(!position_top.is_unset() && position_type.is_unset())
	const_cast<css *>(this)->css_position_type(pos_absolute, position_top.get_inheritance());
    if(!position_left.is_unset() && position_type.is_unset())
	const_cast<css *>(this)->css_position_type(pos_absolute, position_left.get_inheritance());
    if(!position_bottom.is_unset() && position_type.is_unset())
	const_cast<css *>(this)->css_position_type(pos_absolute, position_bottom.get_inheritance());
    if(!position_right.is_unset() && position_type.is_unset())
	const_cast<css *>(this)->css_position_type(pos_absolute, position_right.get_inheritance());

    if(!position_top.is_unset()
       || !position_left.is_unset()
       || !position_bottom.is_unset()
       || !position_right.is_unset())
    {
	ret += position_type.get_value();
	ret += position_top.get_value();
	ret += position_left.get_value();
	ret += position_bottom.get_value();
	ret += position_right.get_value();
    }

    ret += z_index.get_value();
    ret += overflow.get_value();
    ret += float_pos.get_value();
    ret += float_clear.get_value();
    ret += opacity.get_value();
    ret += padding_top.get_value();
    ret += padding_right.get_value();
    ret += padding_bottom.get_value();
    ret += padding_left.get_value();
    ret += font_size.get_value();
    ret += font_style.get_value();
    ret += font_weight.get_value();
    ret += text_h_align.get_value();
    ret += text_v_align.get_value();
    ret += text_deco.get_value();
    ret += border_width.get_value();
    ret += border_color.get_value();
    ret += border_style.get_value();
    ret += corner_radius.get_value();
    ret += visibility.get_value();

    map<string, css_property>::const_iterator it = custom_css.begin();

    while(it != custom_css.end())
    {
	ret += it->second.get_value();
	++it;
    }

    return ret;
}

void css::declare_custom_css(const string & label)
{
    map<string, css_property>::iterator it = custom_css.find(label);

    if(it != custom_css.end())
	throw exception_range(string("custom_css already declared for this css object: ") + label);

    custom_css[label] = css_property();
}

void css::set_custom_css(const string & label, const string & val, bool inherit)
{
    map<string, css_property>::iterator it = custom_css.find(label);

    if(it == custom_css.end())
	throw exception_range(string("cannot set an undeclared custom css: ") + label);

    it->second.set_value(val);
    it->second.set_inheritance(inherit);
    css_updated(inherit);
}

void css::clear_custom_css(const string & label)
{
    map<string, css_property>::iterator it = custom_css.find(label);

    if(it == custom_css.end())
	throw exception_range(string("cannot set an undeclared custom css: ") + label);

    it->second.clear();
}

string css::border_to_string(border val)
{
    string ret;

    switch(val)
    {
    case bd_top:
	ret = "-top-";
	break;
    case bd_right:
	ret = "-right-";
	break;
    case bd_bottom:
	ret = "-bottom-";
	break;
    case bd_left:
	ret = "-left-";
	break;
    case bd_all:
    case bd_clear:
	ret = "-";
	break;
    default:
	throw WEBDAR_BUG;
    }

    return ret;
}

string css::overflow_to_string(overflowing val)
{
    string arg;

    switch(val)
    {
    case ov_visible:
	arg = "visible;";
	break;
    case ov_hidden:
	arg = "hidden;";
	break;
    case ov_scroll:
	arg = "scroll;";
	break;
    case ov_auto:
	arg = "auto;";
	break;
    case ov_inherit:
	arg = "inherit;";
	break;
    default:
	throw WEBDAR_BUG;
    }

    return arg;
}
