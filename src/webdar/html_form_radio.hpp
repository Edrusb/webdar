#ifndef HTML_FORM_RADIO_HPP
#define HTML_FORM_RADIO_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <vector>

    // webdar headers
#include "body_builder.hpp"

class html_form_radio : public body_builder
{
public:
    html_form_radio() { selected = 0; };
    void add_choice(const std::string & id, const std::string & label);
    void clear() { choices.clear(); };

    void set_selected(unsigned int x) ;
    const std::string & get_selected_id() const { return choices[selected].id; };
    unsigned int get_selected_num() const { return selected; };

	// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

protected:
    struct record
    {
	std::string id;
	std::string label;
    };

    const std::vector<record> & get_choices() const { return choices; };
    void update_field_from_request(const request & req);

private:
    std::vector<record> choices;
    unsigned int selected;
};


#endif
