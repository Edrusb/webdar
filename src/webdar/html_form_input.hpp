#ifndef HTML_FORM_INPUT_HPP
#define HTML_FORM_INPUT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <vector>

    // webdar headers
#include "body_builder.hpp"

class html_form_input : public body_builder
{
public:
    enum input_type { text, password, number, range, check };

    html_form_input(const std::string & label,
		    input_type type,
		    const std::string & initial_value, //< for check set empty string for unchecked, anything else for checked
		    unsigned int size);

    void set_range(int min, int max);

    const std::string & get_value() const { return x_init; };
    void set_value(const std::string & val) { x_init = val; };
    const bool get_value_as_bool() const { return !x_init.empty(); }; //< for checkbox empty string means unchecked, anything else checked
    void set_value_as_bool(bool val) { x_init = val ? "x" : ""; };

    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    std::string x_label; //< field text shown to the user
    std::string x_type;  //< type of HTML input field
    std::string x_init;  //< current value / initial value of the field
    std::string x_size;  //< width of the field in number of characterds
    std::string x_min;   //< minimum value of the field (for type = number)
    std::string x_max;   //< maximum value of the field (for type = number)
};

#endif
