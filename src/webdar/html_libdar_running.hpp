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
#include "web_user_interaction.hpp"
#include "html_statistics.hpp"
#include "html_button.hpp"

class html_libdar_running : public html_page, public events
{
public:
    static const std::string ask_end_libdar;      //< ask_close button has been pressed
    static const std::string force_end_libdar;    //< force_close button has been pressed
    static const std::string close_libdar_screen; //< finish button has been pressed

	/// constructor
    html_libdar_running();

	/// clear logs and reset counters
    void clear();

	/// be informed of the end of libdar thread
    void libdar_has_finished() { set_mode(finished); set_title("Libdar thread has ended"); };

	/// provide objects for libdar execution
    web_user_interaction & get_user_interaction() { return web_ui; };
    html_statistics & get_statistics() { return stats; };

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    enum mode_type
    {
	normal,        //< should display web_user_interface, progressive_report and ask_close button
	end_asked,     //< should display web_user_interface, progressive_report and force_close button
	end_forced,    //< should display web_user_interface, progressive_report (no button)
	finished       //< should display web_user_interface, progressive_report, close button
    };

    mode_type mode;
    html_div global;
    web_user_interaction web_ui;
    html_statistics stats;
    html_button ask_close;
    html_button force_close;
    html_button finish;

    void set_mode(mode_type m);
};


#endif
