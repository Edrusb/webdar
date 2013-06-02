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

class html_libdar_running : public html_page, public events
{
public:
    static const std::string ask_end_libdar;
    static const std::string force_end_libdar;
    static const std::string close_libdar_screen;

    html_libdar_running();

    void clear(); // clear logs and reset counters

    void libdar_has_finished(); // be informed of the end of libdar thread

    web_user_interaction & get_user_interaction() { return web_ui; };
    html_statistics & get_statistics() { return stats; };

	// gestion du refresh en fonction de webui !!!

private:
    enum
    {
	end_asked,     //< should display web_user_interface, progressive_report and forced cancellation button
	end_forced,    //< should display web_user_interface, progressive_report (no button)
	exceptions,    //< should trigger libdar possible exceptions and update errors field
	finished       //< should display web_user_interface, progressive_report, eventually errors field and close button
    } mode;

    web_user_interaction web_ui;
    html_statistics stats;
    html_button ask_close;
    html_button force_close;
    html_button finish;
};


#endif
