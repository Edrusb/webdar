#ifndef HTML_IMAGE_HPP
#define HTML_IMAGE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "static_body_builder.hpp"


class html_image : public body_builder, public static_body_builder
{
public:
    html_image(const std::string & url, const std::string & alt);

    void set_dimensions(const std::string x, const std::string y) { dim_x = x; dim_y = y; };

    virtual std::string get_body_part(const chemin & path,
				      const request & req) { return get_body_part(); };
    virtual std::string get_body_part() const;

private:
    std::string x_url;
    std::string x_alt;
    std::string dim_x;
    std::string dim_y;
};


#endif
