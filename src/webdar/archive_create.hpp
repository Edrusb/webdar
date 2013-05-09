#ifndef ARCHIVE_CREATE_HPP
#define ARCHIVE_CREATE_HPP

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

class archive_create: public body_builder, public actor
{
public:
    archive_create();
    archive_create(const archive_create & ref);
    const archive_create & operator = (const archive_create & ref) { throw WEBDAR_BUG; };

    void create_archive(/* webdinteraction required here */);

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// inherited from on_event
    virtual void on_event(const std::string & event_name);

private:
    html_form_input fs_root;
    html_form_input sauv_path;
    html_form_input basename;
    html_form_input show_options;
    options_create options;
    html_form form;
};

#endif
