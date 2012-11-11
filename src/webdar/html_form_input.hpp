#ifndef HTML_FORM_INPUT_HPP
#define HTML_FORM_INPUT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"

class html_form_input : public body_builder
{
public:
    enum input_type { text, password, number, range, check };

    html_form_input(const std::string & id,
		    const std::string & label,
		    input_type type,
		    const std::string & initial_value, //< for check set empty string for unchecked , anything else for checked
		    unsigned int size);

    void set_range(int min, int max);

    const std::string & get_value() const { return x_init; };

    virtual std::string get_body_part(const std::deque<std::string> & path,
				      const request & req);

private:
    std::string x_id;
    std::string x_label;
    std::string x_type;
    std::string x_init;
    std::string x_size;
    std::string x_min;
    std::string x_max;
};

#endif
