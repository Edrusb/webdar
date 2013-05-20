#ifndef HTML_ARCHIVE_READ_HPP
#define HTML_ARCHIVE_READ_HPP

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

class html_archive_read: public body_builder, public actor
{
public:
    html_archive_read(const std::string & archive_description);

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// inherited from events
    virtual void on_event(const std::string & event_name);

	// available fields for libdar

    const std::string & get_archive_path() const { return arch_path.get_value(); };
    const std::string & get_archive_basename() const { return archive.get_value(); };
    const libdar::archive_options_read get_read_options() const { opt_read.get_options(); };

private:
    html_form form;
    html_form_fieldset fs;
    html_form_input arch_path;
    html_form_input archive;
    html_form_input show_read_options;
    options_read opt_read;
};

#endif
