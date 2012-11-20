#ifndef CSS_HPP
#define CSS_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "css_property.hpp"

class css
{
public:
    css() { css_clear_attributes(); };
    virtual ~css() {};

    void css_clear_attributes();

	// colors

    void css_color(const std::string & col,
		   bool inherit=false);
    void css_background_color(const std::string & col,
			      bool inherit=false);

	// background image

    void css_background_image(const std::string & url,
			      bool repeat_x,
			      bool repeat_y,
			      bool fixed,
			      bool inherit=false);
    void set_css_background_position(const std::string & x,
				     const std::string & y,
				     bool inherit=false);

	// position in the window

    void css_margin_top(const std::string & top,
			bool inherit=false);
    void css_margin_right(const std::string & right,
			  bool inherit=false);
    void css_margin_bottom(const std::string & bottom,
			   bool inherit=false);
    void css_margin_left(const std::string & left,
			 bool inherit=false);
    void css_height(const std::string & val,
		    bool center,
		    bool inherit=false);
    void css_width(const std::string & val,
		   bool center,
		   bool inherit=false);
    void css_float_left(bool inherit=false);
    void css_float_right(bool inherit=false);
    void css_float_none(bool inherit=false);
    void css_float_clear_left(bool inherit=false);
    void css_float_clear_right(bool inherit=false);
    void css_float_clear_both(bool inherit=false);
    void css_float_clear_none(bool inherit=false);
    void css_padding_top(const std::string & top,
			 bool inherit=false);
    void css_padding_right(const std::string & right,
			   bool inherit=false);
    void css_padding_bottom(const std::string & bottom,
			    bool inherit=false);
    void css_padding_left(const std::string & left,
			  bool inherit=false);


	/// inherit properties from the given reference
	///
	/// only unset properties are set to the value of the given reference
    void css_inherit_from(const css & ref);

protected:
    std::string get_css_string() const;

private:
    css_property color;
    css_property bg_col;
    css_property bg_img;
    css_property img_pos;
    css_property margin_top;
    css_property margin_right;
    css_property margin_bottom;
    css_property margin_left;
    css_property height;
    css_property width;
    css_property float_pos;
    css_property float_clear;
    css_property padding_top;
    css_property padding_right;
    css_property padding_bottom;
    css_property padding_left;
};

#endif
