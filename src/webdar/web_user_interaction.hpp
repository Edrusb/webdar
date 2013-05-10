#ifndef WEB_USER_INTERACTION_HPP
#define WEB_USER_INTERACTION_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_form_input.hpp"
#include "html_form.hpp"
#include "html_form_radio.hpp"
#include "html_form_fieldset.hpp"
#include "html_text.hpp"
#include "events.hpp"
#include "semaphore.hpp"

struct web_user_interaction_libdar_data
{
    mutex control;          //< control access to any value this object has
    unsigned int instances; //< number of reference existing toward this object, this object data get destroyed when this number drops to zero
    semaphore libdar_sem;   //< true if a pause2() is pending for a response
    bool answered;          //< true if the pending pause2(), get_string() or get_secu_string() has been answered, so the question has not to be shown twice

	// pause2()
    bool pause2_pending;    //< true if a pause2() is pending for a response
    std::string pause2_msg; //< the request to answser to
    bool pause2_ans;        //< the answer to the request

	// get_string() / get_secu_string()
    bool get_string_pending;//< true if a get_string() is pending for a response
    std::string get_string_msg;
    bool get_string_echo;
    std::string get_string_ans;
	// get_secu_string()
    bool get_secu_string_pending; //< true if a get_secu_string() is pending for a response
    std::string get_secu_string_msg;
    bool get_secu_string_echo;
    libdar::secu_string get_secu_string_ans;
	// warnings
    std::list<std::string> warnings;
    unsigned warn_size;
};

struct web_user_interaction_html_data
{
    html_form_radio pause2;
    html_form_fieldset inter;
    html_form_input get_string;
    html_form form;
    html_text warnings;
    html_form_fieldset logs;
    html_form_fieldset global;
    bool rebuild_body_part;

    web_user_interaction_html_data();
};


class web_user_interaction: public libdar::user_interaction, public body_builder, public actor
{
public:
    web_user_interaction(unsigned int x_warn_size);
    web_user_interaction(const web_user_interaction & ref) { copy_from(ref); };
    const web_user_interaction & operator = (const web_user_interaction & ref) { destroy(); copy_from(ref); };
    ~web_user_interaction() { destroy(); };

	/// change the number of last warnings to display
    void set_warning_list_size(unsigned int size);

	/// inherited from libdar::user_interaction, these are called by the libdar thread
    virtual bool pause2(const std::string & message);
    virtual std::string get_string(const std::string & message, bool echo);
    virtual libdar::secu_string get_secu_string(const std::string & message, bool echo);
    virtual libdar::user_interaction *clone() const;

	/// inherited from body_builder, called by the webdar thread
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// inherited from actor
    virtual void on_event(const std::string & event_name);

protected:
	// inherited from libdar::user_interaction
    virtual void inherited_warning(const std::string & message);

private:
    web_user_interaction_libdar_data *lib_data;
    web_user_interaction_html_data *html_data;

    void copy_from(const web_user_interaction & ref);
    void destroy();
};

#endif
