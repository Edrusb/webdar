#ifndef HTML_FORM_HPP
#define HTML_FORM_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html.hpp"


class html_form : public html
{
public:
    html_form(const std::string & target,
	      const std::string & validate_msg = "send");

    void add_field(const std::string & val) { fields += val + "\n"; };

    void clear() { fields = ""; };

    virtual std::string display() const;

private:
    std::string url;
    std::string fields;
    std::string go_mesg;
};

#endif
