#ifndef OPTIONS_COMPARE_HPP
#define OPTIONS_COMPARE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "html_form_input.hpp"
#include "html_form_fieldset.hpp"
#include "html_comparison_fields.hpp"
#include "html_form.hpp"

class options_compare : public body_builder
{
public:
    options_compare();

    libdar::archive_options_diff get_options() const;

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);


private:
    html_form form;
    html_form_fieldset fs;
    html_form_input info_details;
    html_comparison_fields what_to_check;
    html_form_input alter_atime;
    html_form_input furtive_read_mode;
    html_form_input display_skipped;
    html_form_input hourshift;
    html_form_input compare_symlink_date;

};


#endif
