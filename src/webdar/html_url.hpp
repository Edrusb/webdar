#ifndef HTML_URL_HPP
#define HTML_URL_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include "html.hpp"

    // webdar headers

class html_url : public html
{
public:
    html_url(const std::string & url, const std::string & label) { x_url = url; x_label = label; };

    virtual std::string display() const;

private:
    std::string x_url;
    std::string x_label;
};


#endif
