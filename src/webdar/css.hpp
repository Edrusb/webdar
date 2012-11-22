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

    void css_margin(const std::string & all,
		    bool inherit=false);
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

	// floating

    enum floating { fl_left,
		    fl_right,
		    fl_none };

    enum floatclear { fc_left,
		      fc_right,
		      fc_both,
		      fc_none };

    void css_float(floating val, bool inherit=false);
    void css_float_clear(floatclear val, bool inherit=false);

	// padding

    void css_padding(const std::string & val,
		     bool inherit=false);
    void css_padding_top(const std::string & top,
			 bool inherit=false);
    void css_padding_right(const std::string & right,
			   bool inherit=false);
    void css_padding_bottom(const std::string & bottom,
			    bool inherit=false);
    void css_padding_left(const std::string & left,
			  bool inherit=false);

	// text

    enum align { al_right,
		 al_left,
		 al_center,
		 al_justify };

    enum decoration { dc_underline,
		      dc_overline,
		      dc_line_through };

    void css_font_style_italic(bool inherit=false);
    void css_font_style_normal(bool inherit=false);
    void css_font_weight_bold(bool inherit=false);
    void css_font_weight_normal(bool inherit=false);
    void css_text_align(align val, bool inherit=false);
    void css_text_decoration(decoration val, bool inherit=false);

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
    void css_border_color(border which, const std::string & col, bool inherit=false);
    void css_border_style(border which, bd_style val, bool inherit=false);


	/// inherit properties from the given reference
	///
	/// \note only unset properties are set to the value of the given reference
	/// and only inheritable properties of the references are copied
    void css_inherit_from(const css & ref);

protected:
    std::string get_css_string() const;

	// this method is called when a property has been modified with inheritance set
    virtual void css_updated() {};

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

    std::string border_to_string(border val);
};

#endif
