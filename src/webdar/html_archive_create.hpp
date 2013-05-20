#ifndef HTML_ARCHIVE_CREATE_HPP
#define HTML_ARCHIVE_CREATE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_input.hpp"
#include "options_create.hpp"
#include "body_builder.hpp"
#include "events.hpp"

class html_archive_create: public body_builder, public actor
{
public:
    html_archive_create();
    html_archive_create(const html_archive_create & ref);
    const html_archive_create & operator = (const html_archive_create & ref) { throw WEBDAR_BUG; };

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// inherited from on_event
    virtual void on_event(const std::string & event_name);

    const std::string & get_archive_path() const { return sauv_path.get_value(); };
    const std::string & get_archive_basename() const { return basename.get_value(); };
    const options_create & get_options_create() const { return options; };

private:
    html_form_input fs_root;
    html_form_input sauv_path;
    html_form_input basename;
    html_form_input show_options;
    options_create options;
    html_form form;
};

#endif
