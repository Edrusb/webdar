#ifndef CSS_HPP
#define CSS_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers


class css
{
public:
    css() { color = bg_col = bg_img = ""; };
    virtual ~css() {};

    void clear_css_attributes();
    void set_css_color(const std::string & col) { color = col; };
    void set_css_background_color(const std::string & col) { bg_col = col; };
    void set_css_background_image(const std::string & url,
			      bool repeat_x,
			      bool repeat_y,
			      bool fixed)
    {
	bg_img = url; img_x = repeat_x; img_y = repeat_y; img_fixed = fixed;
	img_pos_x = img_pos_y = "";
    };

    void set_css_background_position(const std::string & x, const std::string & y)
    { img_pos_x = x; img_pos_y = y; };

protected:
    std::string get_css_string() const;

private:
    std::string color;
    std::string bg_col;
    std::string bg_img;
    bool img_x;
    bool img_y;
    bool img_fixed;
    std::string img_pos_x;
    std::string img_pos_y;
};

#endif
