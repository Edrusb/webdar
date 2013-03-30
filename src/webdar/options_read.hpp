#ifndef OPTIONS_READ_HPP
#define OPTIONS_READ_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_div.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_select.hpp"
#include "html_form_input.hpp"
#include "html_crypto_algo.hpp"
#include "events.hpp"

class options_read : public body_builder, public actor
{
public:
    options_read();

    libdar::archive_options_read get_options() const;

	/// whether the HTML control shows or not
    void set_visible(bool val) { visible = val; };

	/// inherited from actor
    void on_event(const std::string & event_name);

	/// inherited from body_builder
    std::string get_body_part(const chemin & path,
			      const request & req);

private:
    bool visible;

	// construction fields
    html_div div;
    html_form form_src;
    html_form_fieldset fs_src;
    html_form_fieldset fs_ref;
	// archive_options_read fields
    html_crypto_algo src_crypto_algo;
    html_form_input src_crypto_pass;
    html_form_input src_crypto_size;
    html_form_input src_execute;
    html_form_input src_slice_min_digits;
    html_form_input info_details;
    html_form_input lax;
    html_form_input sequential_read;
	// html_select src_entrepot;
    html_form_input src_use_external_catalogue;
    html_form form_ref;
    html_form_input ref_path;
    html_form_input ref_basename;
    html_crypto_algo ref_crypto_algo;
    html_form_input ref_crypto_pass;
    html_form_input ref_crypto_size;
    html_form_input ref_execute;
    html_form_input ref_slice_min_digits;
	// html_select ref_entrepot;
};

#endif
