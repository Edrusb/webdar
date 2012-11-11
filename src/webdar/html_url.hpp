#ifndef HTML_URL_HPP
#define HTML_URL_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include "body_builder.hpp"
#include "static_body_builder.hpp"

    // webdar headers

class html_url : public body_builder, public static_body_builder
{
public:
    html_url(const std::string & url, const std::string & label) { x_url = url; x_label = label; };

    virtual std::string get_body_part(const chemin & path,
				      const request & req) { return get_body_part(); };

    virtual std::string get_body_part() const;

private:
    std::string x_url;
    std::string x_label;
};


#endif
