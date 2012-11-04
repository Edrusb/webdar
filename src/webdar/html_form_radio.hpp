#ifndef HTML_FORM_RADIO_HPP
#define HTML_FORM_RADIO_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <vector>

    // webdar headers
#include "html.hpp"

class html_form_radio : public html
{
public:
    html_form_radio(const std::string & id) { x_id = id; };
    void add_choice(const std::string & id, const std::string & label);
    void clear() { choices.clear(); };

    virtual std::string display() const;

protected:
    struct record
    {
	std::string id;
	std::string label;
    };

    const std::string & get_id() const { return x_id; };
    const std::vector<record> & get_choices() const { return choices; };

private:
    std::string x_id;
    std::vector<record> choices;
};


#endif
