#ifndef OPTIONS_TEST_HPP
#define OPTIONS_TEST_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_input.hpp"

class options_test : public body_builder
{
public:
    options_test();

    libdar::archive_options_test get_options() const;

	/// inherited from bdy_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    html_form form;
    html_form_fieldset fs;
    html_form_input info_details;
    html_form_input empty;
    html_form_input display_skipped;
};

#endif
