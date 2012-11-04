#ifndef HTML_FORM_FIELDSET_HPP
#define HTML_FORM_FIELDSET_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html.hpp"

class html_form_fieldset : public html
{
public:
    html_form_fieldset(const std::string & label) { x_label = label; fields = ""; };

    void add_field(const std::string & val) { fields += val + "\n"; };
    void clear() { fields = ""; };

    virtual std::string display() const;

private:
    std::string x_label;
    std::string fields;

};


#endif
