#ifndef HTML_ARCHIVE_MERGE_HPP
#define HTML_ARCHIVE_MERGE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_input.hpp"
#include "html_form_fieldset.hpp"
#include "html_options_merge.hpp"
#include "body_builder.hpp"
#include "events.hpp"

class html_archive_merge: public body_builder, public actor
{
public:
    html_archive_merge();
    html_archive_merge(const html_archive_merge & ref);
    const html_archive_merge & operator = (const html_archive_merge & ref) { throw WEBDAR_BUG; };

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// inherited from on_event
    virtual void on_event(const std::string & event_name);

    const std::string & get_archive_path() const { return sauv_path.get_value(); };
    const std::string & get_archive_basename() const { return basename.get_value(); };
    const html_options_merge & get_options_merge() const { return options; };
    const html_archive_read & get_reference() const { return reference; };

private:
    html_form form;
    html_form_fieldset fs;
    html_form_input sauv_path;
    html_form_input basename;
    html_form_input show_options;
    html_archive_read reference;
    html_options_merge options;

};

#endif
