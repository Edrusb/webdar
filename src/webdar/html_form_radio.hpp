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
#include "events.hpp"

class html_form_radio : public body_builder, public events
{
public:
    static std::string changed;

    html_form_radio() { selected = 0; visible = true; register_name(changed); };
    void add_choice(const std::string & id, const std::string & label);
    void clear() { choices.clear(); selected = 0; };

    void set_selected(unsigned int x);
    const std::string & get_selected_id() const { return choices[selected].id; };
    unsigned int get_selected_num() const { return selected; };

	/// whether the HTML control shows or not
    void set_visibile(bool val) { visible = val; };

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
    bool is_visible() const { return visible; };

private:
    std::vector<record> choices;
    unsigned int selected;
    bool visible;
};


#endif
