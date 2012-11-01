#ifndef HTML_HPP
#define HTML_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers


class html
{
public:
    html() { color = bg_col = bg_img = ""; };
    virtual ~html() {};


    void clear_css_attributes();
    void set_color(const std::string & col) { color = col; };
    void set_background_color(const std::string & col) { bg_col = col; };
    void set_background_image(const std::string & url,
			      bool repeat_x,
			      bool repeat_y,
			      bool fixed)
    {
	bg_img = url; img_x = repeat_x; img_y = repeat_y; img_fixed = fixed;
	img_pos_x = img_pos_y = "";
    };

    void set_background_position(const std::string & x, const std::string & y)
    { img_pos_x = x; img_pos_y = y; };

	/// common method to produce HTML content
    virtual std::string display() const = 0;

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
