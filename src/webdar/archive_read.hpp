#ifndef ARCHIVE_READ_HPP
#define ARCHIVE_READ_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "html_form.hpp"
#include "html_form_input.hpp"
#include "html_form_fieldset.hpp"
#include "options_read.hpp"
#include "events.hpp"
#include "web_user_interaction.hpp"

class archive_read: public body_builder, public actor
{
public:
    archive_read(const std::string & archive_description);
    archive_read(const archive_read & ref);
    const archive_read & operator = (const archive_read & ref) { throw WEBDAR_BUG; };
    ~archive_read() { if(ptr != NULL) close_archive(); };

    void open_archive(web_user_interaction & ui);
    bool is_archive_open() const { return ptr != NULL; };
    libdar::archive & get_archive();
    void close_archive();

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// inherited from events
    virtual void on_event(const std::string & event_name);

private:
    html_form form;
    html_form_fieldset fs;
    html_form_input archive;
    html_form_input show_read_options;
    options_read opt_read;

    libdar::archive *ptr;
};

#endif
