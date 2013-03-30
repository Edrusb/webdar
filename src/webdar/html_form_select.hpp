#ifndef HTML_FORM_SELECT_HPP
#define HTML_FORM_SELECT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_radio.hpp"

class html_form_select : public html_form_radio, public actor
{
public:
    static const std::string changed;

    html_form_select(const std::string & label) { x_label = label; register_name(changed); record_actor_on_event(this, html_form_radio::changed); };

    void change_label(const std::string & label) { x_label = label; };

	// inherit from html_form_radio the following methods to feed contents
	// . add_choice
	// . clear
	// . set_selected
	// . get_selected_id
	// . get_selected_num

    virtual std::string get_body_part(const chemin & path,
				      const request & req);


	/// inherited from actor
    virtual void on_event(const std::string & event_name) { act(changed); };
private:
    std::string x_label;

};



#endif
