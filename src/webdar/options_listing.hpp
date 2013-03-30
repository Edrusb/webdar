#ifndef OPTIONS_LISTING_HPP
#define oOPTIONS_LISTING_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_form_input.hpp"
#include "html_text.hpp"
#include "html_form.hpp"
#include "html_div.hpp"
#include "html_form_fieldset.hpp"


class options_listing : public body_builder
{
public:
    options_listing();

    libdar::archive_options_listing get_options() const;

	/// inherited from body_builder
    std::string get_body_part(const chemin & path,
			      const request & req);

private:
    html_div div;
    html_form form;
    html_form_fieldset fs;
    html_form_input info_details;  // checkbox
    html_form_input filter_unsaved;// checkbox
    html_form_input display_ea;    // checkbox
};


#endif
