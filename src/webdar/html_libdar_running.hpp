#ifndef HTML_LIBDAR_RUNNING_HPP
#define HTML_LIBDAR_RUNNING_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_page.hpp"
#include "events.hpp"
#include "html_div.hpp"
#include "html_web_user_interaction.hpp"
#include "html_statistics.hpp"
#include "html_button.hpp"

class html_libdar_running : public html_page, public events, public actor
{
public:
    static const std::string ask_end_libdar;      //< ask_close button has been pressed (graceful ending requested)
    static const std::string force_end_libdar;    //< force_close button has been pressed (immediate ending requested)
    static const std::string kill_libdar_thread;  //< kill_close button has been pressed (kill thread requested)
    static const std::string close_libdar_screen; //< finish button has been pressed


	/// constructor
    html_libdar_running();

	/// clear logs and reset counters
    void clear();

	/// be informed of the end of libdar thread
    void libdar_has_finished() { set_mode(finished); set_title("Webdar - Libdar has ended"); };

	/// provide objects for libdar execution
    web_user_interaction & get_user_interaction() { return web_ui.get_user_interaction(); };
    html_statistics & get_statistics() { return stats; };

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// inherited from actor
    virtual void on_event(const std::string & event_name);

private:
    enum mode_type
    {
	normal,        //< should display web_user_interface, progressive_report and ask_close button
	end_asked,     //< should display web_user_interface, progressive_report and force_close button
	end_forced,    //< should display web_user_interface, progressive_report and kill_close button
	kill_forced,   //< should display web_user_interface, progressive_report (no button)
	finished       //< should display web_user_interface, progressive_report, close button
    };

    mode_type mode;
    html_div global;
    html_web_user_interaction web_ui;
    html_statistics stats;
    html_button ask_close;
    html_button force_close;
    html_button kill_close;
    html_button finish;
    bool visibility_has_changed; //< true whether a html component had its visibility changed

    void set_mode(mode_type m);
};


#endif

