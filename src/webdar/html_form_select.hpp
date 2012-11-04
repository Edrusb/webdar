#ifndef HTML_FORM_SELECT_HPP
#define HTML_FORM_SELECT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_radio.hpp"

class html_form_select : public html_form_radio
{
public:
    html_form_select(const std::string & id, const std::string & label):html_form_radio(id) { x_label = label; };

    virtual std::string display() const;

private:
    std::string x_label;

};



#endif
