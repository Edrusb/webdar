#ifndef OPTIONS_EXTRACT_HPP
#define OPTIONS_EXTRACT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_form_select.hpp"
#include "html_form_input.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"

class options_extract : public body_builder
{
public:
    options_extract();

    libdar::archive_options_extract get_options() const;

	/// inherited from body_builder
    std::string get_body_part(const chemin & path,
			      const request & req);
private:
    html_form form;
    html_form_fieldset fs;
	// selection (filter)
	// subtree (filter)
    html_form_input warn_over;
    html_form_input info_details;
	// ea_mask (filter)
    html_form_input flat;
    html_form_select what_to_check;
    html_form_input warn_remove_no_match;
    html_form_input empty;
    html_form_input display_skipped;
    html_form_input empty_dir;
    html_form_select dirty_behavior;
	// overwriting_rules (policy)
    html_form_input only_deleted;
    html_form_input ignore_deleted;
};

#endif
