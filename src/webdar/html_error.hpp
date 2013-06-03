#ifndef HTML_ERROR_HPP
#define HTML_ERROR_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_page.hpp"
#include "events.hpp"
#include "html_text.hpp"
#include "html_button.hpp"

class html_error : public html_page, public events, public actor
{
public:
    static const std::string acknowledged;

    html_error();

    void set_message(const std::string & msg);

	/// inherited from actor class
    virtual void on_event(const std::string & event_name);

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    html_div global;
    html_text the_error;
    html_button close;

    static const std::string close_event;
};

#endif
