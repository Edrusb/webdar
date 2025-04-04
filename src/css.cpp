/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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


void css::update_from(const css & ref)
{
    color.update_from(ref.color);
    bg_col.update_from(ref.bg_col);
    bg_img.update_from(ref.bg_img);
    img_pos.update_from(ref.img_pos);
    box_shadow.update_from(ref.box_shadow);
    text_shadow.update_from(ref.text_shadow);
    box_sizing.update_from(ref.box_sizing);
    display.update_from(ref.display);
    margin_top.update_from(ref.margin_top);
    margin_right.update_from(ref.margin_right);
    margin_bottom.update_from(ref.margin_bottom);
    margin_left.update_from(ref.margin_left);
    height.update_from(ref.height);
    min_height.update_from(ref.min_height);
    max_height.update_from(ref.max_height);
    width.update_from(ref.width);
    min_width.update_from(ref.min_width);
    max_width.update_from(ref.max_width);
    z_index.update_from(ref.z_index);
    position_type.update_from(ref.position_type);
    position_top.update_from(ref.position_top);
    position_left.update_from(ref.position_left);
    position_bottom.update_from(ref.position_bottom);
    position_right.update_from(ref.position_right);
    overflow.update_from(ref.overflow);
    float_pos.update_from(ref.float_pos);
    float_clear.update_from(ref.float_clear);
    opacity.update_from(ref.opacity);
    padding_top.update_from(ref.padding_top);
    padding_right.update_from(ref.padding_right);
    padding_bottom.update_from(ref.padding_bottom);
    padding_left.update_from(ref.padding_left);
    font_size.update_from(ref.font_size);
    font_style.update_from(ref.font_style);
    font_weight.update_from(ref.font_weight);
    text_h_align.update_from(ref.text_h_align);
    text_v_align.update_from(ref.text_v_align);
    text_deco.update_from(ref.text_deco);
    border_width.update_from(ref.border_width);
    border_color.update_from(ref.border_color);
    border_style.update_from(ref.border_style);
    corner_radius.update_from(ref.corner_radius);
    visibility.update_from(ref.visibility);
    grid_template.update_from(ref.grid_template);
    grid_template_lines.update_from(ref.grid_template_lines);
    grid_area.update_from(grid_area);

    map<string, css_property>::const_iterator it = ref.custom_css.begin();
    map<string, css_property>::iterator mit;
    while(it != ref.custom_css.end())
    {
	if(! it->second.is_unset())
	{
	    mit = custom_css.find(it->first);
	    if(mit == custom_css.end()) // not such property in 'this'
	    {
		declare_custom_css(it->first);
		mit = custom_css.find(it->first);
		if(mit == custom_css.end())
		    throw WEBDAR_BUG;
	    }

	    mit->second.update_from(it->second);
	}

	++it;
    }
}


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
    min_height.clear();
    max_height.clear();
    width.clear();
    min_width.clear();
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
    visibility.clear();
    grid_template.clear();
    grid_template_lines.clear();
    grid_area.clear();

    map<string, css_property>::iterator it = custom_css.begin();
    while(it != custom_css.end())
    {
	it->second.clear();
	++it;
    }
}

void css::css_color(const string & col)
{
    color.set_value(string(" color: ") + col + ";");
}

void css::css_background_color(const string & col)
{
    bg_col.set_value(string(" background-color: ") + col + ";");
}


void css::css_background_image(const string & url,
			       bool repeat_x,
			       bool repeat_y,
			       bool fixed)
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
}

void css::css_background_position(const string & x,
				  const string & y)
{
    if(x != "" && y != "") // yes both
	img_pos.set_value(string(" background-position: ") + x + " " + y + ";");
    else
	img_pos.clear();
}

void css::css_box_shadow(const string & x_shift,
			 const string & y_shift,
			 const string & blur_size,
			 const string & color)
{
    box_shadow.set_value(string(" box-shadow: ")
			 + x_shift + " "
			 + y_shift + " "
			 + blur_size + " "
			 + color + ";");
}

void css::css_text_shadow(const string & x_shift,
			  const string & y_shift,
			  const string & blur_size,
			  const string & color)
{
    text_shadow.set_value(string(" text-shadow: ")
			  + x_shift + " "
			  + y_shift + " "
			  + blur_size + " "
			  + color + ";");
}

void css::css_box_sizing(bx_sizing val)
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
}

void css::css_display(const string & val)
{
    display.set_value(string(" display: ") + val + ";");
}

void css::css_margin(const string & all)
{
    css_margin_top(all);
    css_margin_right(all);
    css_margin_bottom(all);
    css_margin_left(all);
}

void css::css_margin_top(const string & top)
{
    margin_top.set_value(string(" margin-top: ") + top + ";");
}

void css::css_margin_right(const string & right)
{
    margin_right.set_value(string(" margin-right: ") + right + ";");

}
void css::css_margin_bottom(const string & bottom)
{
    margin_bottom.set_value(string(" margin-bottom: ") + bottom + ";");
}

void css::css_margin_left(const string & left)
{
    margin_left.set_value(string(" margin-left: ") + left + ";");
}

void css::css_height(const string & val,
		     bool center)
{
    height.set_value(string(" height: ") + val + ";");
    if(center)
    {
	css_margin_top("auto");
	css_margin_bottom("auto");
    }
}

void css::css_min_height(const string & val)
{
    min_height.set_value(string(" min-height: ") + val + ";");
}

void css::css_max_height(const string & val)
{
    max_height.set_value(string(" max-height: ") + val + ";");
}

void css::css_width(const string & val,
		    bool center)
{
    width.set_value(string(" width: ") + val + ";");
    if(center)
    {
	css_margin_left("auto");
	css_margin_right("auto");
    }
}

void css::css_min_width(const string & val)
{
    min_width.set_value(string(" min-width: ") + val + ";");
}

void css::css_max_width(const string & val)
{
    max_width.set_value(string(" max-width: ") + val + ";");
}

void css::css_z_index(unsigned int index)
{
    z_index.set_value(string(" z-index: ") + webdar_tools_convert_to_string(index) + ";");
}

void css::css_position_type(positionning val)
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
}


void css::css_position_top(const string & top)
{
    position_top.set_value(string(" top:") + top + ";");
}

void css::css_position_left(const string & left)
{
    position_left.set_value(string(" left:") + left + ";");
}

void css::css_position_bottom(const string & bottom)
{
    position_bottom.set_value(string(" bottom:") + bottom + ";");
}

void css::css_position_right(const string & right)
{
    position_right.set_value(string(" right:") + right + ";");
}

void css::css_overflow(overflowing val)
{
    string arg = " overflow: " + overflow_to_string(val);

    overflow.set_value(arg);
}

void css::css_overflow_x(overflowing val)
{
    string arg = " overflow-x: " + overflow_to_string(val);

    overflow.set_value(arg);
}

void css::css_overflow_y(overflowing val)
{
    string arg = " overflow-y: " + overflow_to_string(val);

    overflow.set_value(arg);
}

void css::css_float(floating val)
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
}

void css::css_float_clear(floatclear val)
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
}

void css::css_opacity(const string & val)
{
    opacity.set_value(string(" opacity: ") + val + ";");
}

void css::css_padding(const string & val)
{
    css_padding_top(val);
    css_padding_right(val);
    css_padding_bottom(val);
    css_padding_left(val);
}

void css::css_padding_top(const string & top)
{
    padding_top.set_value(string(" padding-top: ") + top + ";");
}

void css::css_padding_right(const string & right)
{
    padding_right.set_value(string(" padding-right: ") + right + ";");
}

void css::css_padding_bottom(const string & bottom)
{
    padding_bottom.set_value(string(" padding-bottom: ") + bottom + ";");
}

void css::css_padding_left(const string & left)
{
    padding_left.set_value(string(" padding-left: ") + left + ";");
}

void css::css_font_size(const string & val)
{
    font_size.set_value(string(" font-size: ") + val + ";");
}

void css::css_font_style_italic()
{
    font_style.set_value(" font-style: italic;");
}

void css::css_font_style_normal()
{
    font_style.set_value(" font-style: normal;");
}

void css::css_font_weight_bold()
{
    font_weight.set_value(" font-weight: bold;");
}

void css::css_font_weight_normal()
{
    font_weight.set_value(" font-weight: normal;");
}

void css::css_text_h_align(h_align val)
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
}

void css::css_text_v_align(v_align val)
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
}

void css::css_text_decoration(decoration val)
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
}

void css::css_border_width(border which, bd_width val)
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
}

void css::css_border_color(border which, const string & col)
{
    string arg = string(" border") + border_to_string(which) + "color: ";

    arg += col + ";";

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
}

void css::css_border_style(border which, bd_style val)
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
}

void css::css_corner_radius(const string & all)
{
    css_corner_radius(all, all, all, all);
}


void css::css_corner_radius(const string & topleft,
			    const string & topright,
			    const string & botright,
			    const string & botleft)
{
    string arg = " border-radius: ";

    if(topleft == topright
       && botleft == botright
       && topleft == botleft) // all are equal
	arg += topleft + ";";
    else
	arg += topleft + " " + topright + " " + botright + " " + botleft + ";";

    corner_radius.set_value(arg);
}

void css::css_visibility(bool val)
{
    string text_val = val ? "visible" : "hidden";

    visibility.set_value(string(" visibility: ") + text_val + ";");
}

void css::css_grid_template(const string & gap)
{
    string tmp = " display:grid; gap: " + gap + "; grid-template-areas:";
    grid_template.set_value(tmp);
}

void css::css_grid_template_add_line(const string & line)
{
    string tmp = grid_template_lines.get_value();

    tmp += " '" + line + "'";  // no ending ";" as more line may be added
    grid_template_lines.set_value(tmp);
}

void css::css_grid_area_name(const string & name)
{
    string tmp = " grid-area: " + name + ";";
    grid_area.set_value(tmp);
}

string css::css_get_raw_string() const
{
    string ret = "";
    string tmp;

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
    ret += min_height.get_value();
    ret += max_height.get_value();
    ret += width.get_value();
    ret += min_width.get_value();
    ret += max_width.get_value();

    if(!position_top.is_unset() && position_type.is_unset())
	const_cast<css *>(this)->css_position_type(pos_absolute);
    if(!position_left.is_unset() && position_type.is_unset())
	const_cast<css *>(this)->css_position_type(pos_absolute);
    if(!position_bottom.is_unset() && position_type.is_unset())
	const_cast<css *>(this)->css_position_type(pos_absolute);
    if(!position_right.is_unset() && position_type.is_unset())
	const_cast<css *>(this)->css_position_type(pos_absolute);

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
    ret += grid_template.get_value();
    tmp = grid_template_lines.get_value();
    if(!tmp.empty())
	ret += tmp + ";"; // needed adding ';' after all grid template lines
    ret += grid_area.get_value();

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

void css::set_custom_css(const string & label, const string & val)
{
    map<string, css_property>::iterator it = custom_css.find(label);

    if(it == custom_css.end())
	throw exception_range(string("cannot set an undeclared custom css: ") + label);

    it->second.set_value(val);
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
