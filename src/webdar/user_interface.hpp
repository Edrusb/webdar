#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "responder.hpp"
#include "saisie.hpp"
#include "html_statistics.hpp"
#include "web_user_interaction.hpp"
#include "events.hpp"

class user_interface : public responder, public events, public actor
{
public:
	/// available event for that class
    static const std::string closing;      //< used to signal end of user session
    static const std::string end_libdar;   //< used to signal request to end the running libdar thread
    static const std::string start_restore; //< used to signal request to start a libdar thread
    static const std::string start_compare;
    static const std::string start_test;
    static const std::string start_create;
    static const std::string start_isolate;
    static const std::string start_merge;

    user_interface();

	/// inherited from responder
    virtual answer give_answer(const request & req);

	/// inherited from actor
    virtual void on_event(const std::string & event_name);

	/// available parameters for libdar execution
    const saisie & get_parametrage() const { return parametrage; };

	/// obtain the user_interaction for libdar
    libdar::user_interaction *get_user_interaction() { return &web_ui; };

protected:

	/// inherited from responder
    virtual void prefix_has_changed();

private:
    enum { config, listing, running } mode;
    bool mode_changed;

    saisie parametrage;
    web_user_interaction web_ui;
    html_statistics stats;
    html_button close_click;
    html_div run_div;
};

#endif
