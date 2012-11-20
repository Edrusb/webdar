    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



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
}

void css::css_color(const std::string & col,
		    bool inherit)
{
    color.set_value(string("color: ") + col + ";");
    color.set_inheritance(inherit);
}

void css::css_background_color(const std::string & col,
			       bool inherit)
{
    bg_col.set_value(string("background-color: ") + col + ";");
    bg_col.set_inheritance(inherit);
}


void css::css_background_image(const std::string & url,
			       bool repeat_x,
			       bool repeat_y,
			       bool fixed,
			       bool inherit)
{
    string tmp = "background-repeat: ";
    string val = string("background-image: url(\"") + url + "\");";
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
    val += string(" background-attachment; ") + (fixed ? "fixed;" : "scroll;");

    bg_img.set_value(val);
    bg_img.set_inheritance(inherit);
}


void css::set_css_background_position(const std::string & x,
				      const std::string & y,
				      bool inherit)
{
    if(x != "" && y != "") // yes both
    {
	img_pos.set_value(string("background-position: ") + x + " " + y + ";");
	img_pos.set_inheritance(inherit);
    }
    else
	img_pos.clear();
}

void css::css_margin_top(const std::string & top, bool inherit)
{
    margin_top.set_value(string("margin-top: ") + top + ";");
    margin_top.set_inheritance(inherit);
}

void css::css_margin_right(const std::string & right, bool inherit)
{
    margin_right.set_value(string("margin-right: ") + right + ";");
    margin_right.set_inheritance(inherit);

}
void css::css_margin_bottom(const std::string & bottom, bool inherit)
{
    margin_bottom.set_value(string("margin-bottom: ") + bottom + ";");
    margin_bottom.set_inheritance(inherit);

}

void css::css_margin_left(const std::string & left, bool inherit)
{
    margin_left.set_value(string("margin-left: ") + left + ";");
    margin_left.set_inheritance(inherit);
}

void css::css_height(const std::string & val,
		     bool center,
		     bool inherit)
{
    height.set_value(string("height: ") + val + ";");
    height.set_inheritance(inherit);
    if(center)
    {
	css_margin_top("auto", inherit);
	css_margin_bottom("auto", inherit);
    }
}

void css::css_width(const std::string & val,
		    bool center,
		    bool inherit)
{
    width.set_value(string("width: ") + val + ";");
    width.set_inheritance(inherit);
    if(center)
    {
	css_margin_left("auto", inherit);
	css_margin_right("auto", inherit);
    }
}


void css::css_float_left(bool inherit)
{
    float_pos.set_value("float: left;");
    float_pos.set_inheritance(inherit);
}

void css::css_float_right(bool inherit)
{
    float_pos.set_value("float: right;");
    float_pos.set_inheritance(inherit);
}

void css::css_float_none(bool inherit)
{
    float_pos.set_value("float: none;");
    float_pos.set_inheritance(inherit);
}

void css::css_float_clear_left(bool inherit)
{
    float_clear.set_value("clear: left;");
    float_clear.set_inheritance(inherit);
}

void css::css_float_clear_right(bool inherit)
{
    float_clear.set_value("clear: right;");
    float_clear.set_inheritance(inherit);
}

void css::css_float_clear_both(bool inherit)
{
    float_clear.set_value("clear: both;");
    float_clear.set_inheritance(inherit);
}

void css::css_float_clear_none(bool inherit)
{
    float_clear.set_value("clear: none;");
    float_clear.set_inheritance(inherit);
}

void css::css_padding_top(const std::string & top,
		     bool inherit)
{
    padding_top.set_value(string("paddin-top:") + top + ";");
    padding_top.set_inheritance(inherit);
}

void css::css_padding_right(const std::string & right,
		       bool inherit)
{
    padding_right.set_value(string("paddin-right:") + right + ";");
    padding_right.set_inheritance(inherit);
}
void css::css_padding_bottom(const std::string & bottom,
			bool inherit)
{
    padding_bottom.set_value(string("paddin-bottom:") + bottom + ";");
    padding_bottom.set_inheritance(inherit);
}
void css::css_padding_left(const std::string & left,
		      bool inherit)
{
    padding_left.set_value(string("paddin-left:") + left + ";");
    padding_left.set_inheritance(inherit);
}


void css::css_inherit_from(const css & ref)
{
    color.inherit_from(ref.color);
    bg_col.inherit_from(ref.bg_col);
    bg_img.inherit_from(ref.bg_img);
    img_pos.inherit_from(ref.img_pos);
    margin_top.inherit_from(ref.margin_top);
    margin_right.inherit_from(ref.margin_right);
    margin_bottom.inherit_from(ref.margin_bottom);
    margin_left.inherit_from(ref.margin_left);
    height.inherit_from(ref.height);
    width.inherit_from(ref.width);
    float_pos.inherit_from(ref.float_pos);
    float_clear.inherit_from(ref.float_clear);
    padding_top.inherit_from(ref.padding_top);
    padding_right.inherit_from(ref.padding_right);
    padding_bottom.inherit_from(ref.padding_bottom);
    padding_left.inherit_from(ref.padding_left);
}

string css::get_css_string() const
{
    string ret = "style=\"";

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

    ret += "\"";

    return ret;
}

