    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "exceptions.hpp"


    //
#include "css.hpp"

using namespace std;



void css::css_clear_attributes()
{
    color.clear();
    bg_col.clear();
    bg_img.clear();
    img_pos.clear();
    margin_top.clear();
    margin_right.clear();
    margin_bottom.clear();
    margin_left.clear();
    height.clear();
    width.clear();
    float_pos.clear();
    float_clear.clear();
    padding_top.clear();
    padding_right.clear();
    padding_bottom.clear();
    padding_left.clear();
    font_style.clear();
    font_weight.clear();
    text_align.clear();
    text_deco.clear();
    border_width.clear();
    border_color.clear();
    border_style.clear();
}

void css::css_color(const std::string & col,
		    bool inherit)
{
    color.set_value(string(" color: ") + col + ";");
    color.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_background_color(const std::string & col,
			       bool inherit)
{
    bg_col.set_value(string(" background-color: ") + col + ";");
    bg_col.set_inheritance(inherit);
    css_updated(inherit);
}


void css::css_background_image(const std::string & url,
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

void css::set_css_background_position(const std::string & x,
				      const std::string & y,
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

void css::css_margin(const std::string & all, bool inherit)
{
    css_margin_top(all, inherit);
    css_margin_right(all, inherit);
    css_margin_bottom(all, inherit);
    css_margin_left(all, inherit);
}

void css::css_margin_top(const std::string & top, bool inherit)
{
    margin_top.set_value(string(" margin-top: ") + top + ";");
    margin_top.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_margin_right(const std::string & right, bool inherit)
{
    margin_right.set_value(string(" margin-right: ") + right + ";");
    margin_right.set_inheritance(inherit);
    css_updated(inherit);

}
void css::css_margin_bottom(const std::string & bottom, bool inherit)
{
    margin_bottom.set_value(string(" margin-bottom: ") + bottom + ";");
    margin_bottom.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_margin_left(const std::string & left, bool inherit)
{
    margin_left.set_value(string(" margin-left: ") + left + ";");
    margin_left.set_inheritance(inherit);
    css_updated(inherit);
}

void css::css_height(const std::string & val,
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

void css::css_width(const std::string & val,
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

void css::css_padding_right(const std::string & right,
		       bool inherit)
{
    padding_right.set_value(string(" padding-right: ") + right + ";");
    padding_right.set_inheritance(inherit);
    css_updated(inherit);
}
void css::css_padding_bottom(const std::string & bottom,
			bool inherit)
{
    padding_bottom.set_value(string(" padding-bottom: ") + bottom + ";");
    padding_bottom.set_inheritance(inherit);
    css_updated(inherit);
}
void css::css_padding_left(const std::string & left,
		      bool inherit)
{
    padding_left.set_value(string(" padding-left: ") + left + ";");
    padding_left.set_inheritance(inherit);
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

void css::css_text_align(align val, bool inherit)
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
    text_align.set_value(arg);
    text_align.set_inheritance(inherit);
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

void css::css_border_color(border which, const std::string & col, bool inherit)
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

void css::css_inherit_from(const css & ref, bool all, bool force)
{
    color.inherit_from(ref.color, all, force);
    bg_col.inherit_from(ref.bg_col, all, force);
    bg_img.inherit_from(ref.bg_img, all, force);
    img_pos.inherit_from(ref.img_pos, all, force);
    margin_top.inherit_from(ref.margin_top, all, force);
    margin_right.inherit_from(ref.margin_right, all, force);
    margin_bottom.inherit_from(ref.margin_bottom, all, force);
    margin_left.inherit_from(ref.margin_left, all, force);
    height.inherit_from(ref.height, all, force);
    width.inherit_from(ref.width, all, force);
    float_pos.inherit_from(ref.float_pos, all, force);
    float_clear.inherit_from(ref.float_clear, all, force);
    padding_top.inherit_from(ref.padding_top, all, force);
    padding_right.inherit_from(ref.padding_right, all, force);
    padding_bottom.inherit_from(ref.padding_bottom, all, force);
    padding_left.inherit_from(ref.padding_left, all, force);
    font_style.inherit_from(ref.font_style, all, force);
    font_weight.inherit_from(ref.font_weight, all, force);
    text_align.inherit_from(ref.text_align, all, force);
    text_deco.inherit_from(ref.text_deco, all, force);
    border_width.inherit_from(ref.border_width, all, force);
    border_color.inherit_from(ref.border_color, all, force);
    border_style.inherit_from(ref.border_style, all, force);

    css_updated(true);
}

string css::css_get_raw_string() const
{
    string ret = "";

    ret += color.get_value();
    ret += bg_col.get_value();
    ret += bg_img.get_value();
    ret += img_pos.get_value();
    ret += margin_top.get_value();
    ret += margin_right.get_value();
    ret += margin_bottom.get_value();
    ret += margin_left.get_value();
    ret += height.get_value();
    ret += width.get_value();
    ret += float_pos.get_value();
    ret += float_clear.get_value();
    ret += padding_top.get_value();
    ret += padding_right.get_value();
    ret += padding_bottom.get_value();
    ret += padding_left.get_value();
    ret += font_style.get_value();
    ret += font_weight.get_value();
    ret += text_align.get_value();
    ret += text_deco.get_value();
    ret += border_width.get_value();
    ret += border_color.get_value();
    ret += border_style.get_value();

    return ret;
}

string css::css_get_string() const
{
    string ret = css_get_raw_string();

    if(ret != "")
	ret = "style=\"" + ret + "\"";

    return ret;
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
