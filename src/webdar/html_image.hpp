#ifndef HTML_IMAGE_HPP
#define HTML_IMAGE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html.hpp"



class html_image : public html
{
public:
    html_image(const std::string & url, const std::string & alt);

    void set_dimensions(const std::string x, const std::string y) { dim_x = x; dim_y = y; };

    virtual std::string display() const;

private:
    std::string x_url;
    std::string x_alt;
    std::string dim_x;
    std::string dim_y;
};


#endif
