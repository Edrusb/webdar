#ifndef HTML_WEB_USER_INTERACTION_HPP
#define HTML_WEB_USER_INTERACTION_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "body_builder.hpp"
#include "html_form_input.hpp"
#include "html_form.hpp"
#include "html_form_radio.hpp"
#include "html_form_fieldset.hpp"
#include "html_text.hpp"
#include "events.hpp"
#include "semaphore.hpp"
#include "web_user_interaction.hpp"


class html_web_user_interaction: public body_builder, public actor
{
public:
    html_web_user_interaction(unsigned int x_warn_size = 30);
    html_web_user_interaction(const html_web_user_interaction & ref);
    const html_web_user_interaction & operator = (const html_web_user_interaction & ref) { throw WEBDAR_BUG; };

	/// change the number of last warnings to display
    void set_warning_list_size(unsigned int size) { lib_data.set_warning_list_size(size); };

	/// inherited from body_builder, called by the webdar thread
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// inherited from actor
    virtual void on_event(const std::string & event_name);

	/// clear logs
    void clear() { lib_data.clear(); };

	/// true if no input is requested from libdar, thus HTML refresh
	/// can take place
    bool can_refresh() const { return !lib_data.has_libdar_pending(); };

    web_user_interaction & get_user_interaction() { return lib_data; };

private:
	// fields for exchange with libdar thread
    web_user_interaction lib_data;

	// body_builder fields
    html_form_radio h_pause2;
    html_form_fieldset h_inter;
    html_form_input h_get_string;
    html_form h_form;
    html_text h_warnings;
    html_form_fieldset h_logs;
    html_form_fieldset h_global;
    bool rebuild_body_part;
    bool ignore_event;
};


#endif
