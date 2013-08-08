#ifndef HTML_ARCHIVE_ISOLATE_HPP
#define HTML_ARCHIVE_ISOLATE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_input.hpp"
#include "html_options_isolate.hpp"
#include "body_builder.hpp"
#include "events.hpp"
#include "html_archive_read.hpp"

class html_archive_isolate: public body_builder, public actor
{
public:
    html_archive_isolate();
    html_archive_isolate(const html_archive_isolate & ref);
    const html_archive_isolate & operator = (const html_archive_isolate & ref) { throw WEBDAR_BUG; };

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// inherited from on_event
    virtual void on_event(const std::string & event_name);

    const std::string & get_archive_path() const { return sauv_path.get_value(); };
    const std::string & get_archive_basename() const { return basename.get_value(); };
    const html_options_isolate & get_options_isolate() const { return options; };
    const html_archive_read & get_reference() const { return ref; };
private:
    html_form form;
    html_form_fieldset fs;
    html_form_input sauv_path;
    html_form_input basename;
    html_archive_read ref;
    html_form_input show_options;
    html_options_isolate options;
};

#endif
