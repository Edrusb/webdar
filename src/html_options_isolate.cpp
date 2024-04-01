/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
//
// This file is part of Webdar
//
//  Webdar is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Webdar is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
//
//----
//  to contact the author: dar.linux@free.fr
/*********************************************************************/

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "webdar_css_style.hpp"


    //
#include "html_options_isolate.hpp"

using namespace std;

html_options_isolate::html_options_isolate():
    form_archgen("Update"),
    fs_archgen("General archive isolation options"),
    allow_over("Allow slice overwriting", html_form_input::check, "", 1),
    warn_over("Warn before overwriting", html_form_input::check, "", 1),
    pause("Pause at each N slices (zero = no pause)", html_form_input::number, "", 3),
    slice_permission("Slice permission (octal)", html_form_input::text, "", 10),
    slice_user_ownership("Slice user ownership", html_form_input::text, "", 10),
    slice_group_ownership("slice group ownership", html_form_input::text, "", 10),
    sequential_marks("Add sequential marks", html_form_input::check, "", 1),
    user_comment("User comment in slice header", html_form_input::text, "", 40),
    slice_min_digits("Minimum digits in slice filenames", html_form_input::number, "", 3),
    hash_algo("Hashing algorithm"),
    execute("Command to execute after each slice", html_form_input::text, "", 30),
    empty("Dry run execution", html_form_input::check, "", 1),
    form_shown("Update"),
    fs_shown("What to show during the operation"),
    info_details("Detailed informations", html_form_input::check, "", 1),
    form_compr("Update"),
    fs_compr("Compression options"),
    compression("Compression algorithm"),
    compression_level("Compression level", html_form_input::range, "", 3),
    form_slicing("Update"),
    fs_slicing("Slicing options"),
    slicing("Sliced archive", html_form_input::check, "", 1),
    slice_size("Slice size", html_form_input::number, "", 6),
    different_first_slice("Specific size for first slice", html_form_input::check, "", 1),
    first_slice_size("Slice size", html_form_input::number, "", 6),
    form_crypto("Update"),
    fs_crypto("Encryption options"),
    crypto_algo("Cipher used"),
    crypto_pass1("Pass phrase", html_form_input::password, "", 30),
    crypto_pass2("Confirm pass phrase", html_form_input::password, "", 30),
    crypto_size("Cipher Block size", html_form_input::number, "", 30)
{
    libdar::archive_options_isolate defaults;

	// removing Carriage Return for some components
    compression.set_no_CR();
    slice_size.set_no_CR();
    first_slice_size.set_no_CR();

	// setting default values from libdar
    allow_over.set_value_as_bool(defaults.get_allow_over());
    warn_over.set_value_as_bool(defaults.get_warn_over());
    pause.set_value(libdar::deci(defaults.get_pause()).human());
    slice_permission.set_value(defaults.get_slice_permission());
    slice_user_ownership.set_value(defaults.get_slice_user_ownership());
    slice_group_ownership.set_value(defaults.get_slice_group_ownership());
    sequential_marks.set_value_as_bool(defaults.get_sequential_marks());
    user_comment.set_value(defaults.get_user_comment());
    slice_min_digits.set_value(libdar::deci(defaults.get_slice_min_digits()).human());
    hash_algo.set_value(defaults.get_hash_algo());
    execute.set_value(defaults.get_execute());
    empty.set_value_as_bool(defaults.get_empty());
    info_details.set_value_as_bool(defaults.get_info_details());
    compression.set_value(defaults.get_compression());
    compression_level.set_value(webdar_tools_convert_to_string(defaults.get_compression_level()));
    slicing.set_value_as_bool(defaults.get_slice_size() != 0);
    slice_size.set_value(libdar::deci(defaults.get_slice_size()).human());
    different_first_slice.set_value_as_bool(defaults.get_first_slice_size() != defaults.get_slice_size());
    first_slice_size.set_value(libdar::deci(defaults.get_first_slice_size()).human());
    crypto_algo.set_value(defaults.get_crypto_algo());
    crypto_pass1.set_value("");
    crypto_pass2.set_value("");
    crypto_size.set_value(webdar_tools_convert_to_string(defaults.get_crypto_size()));


	// building HTML structure

    static const char* sect_entrep = "entrepot";
    static const char* sect_general = "general";
    static const char* sect_show = "show";
    static const char* sect_compr = "compress";
    static const char* sect_slice = "slicing";
    static const char* sect_cipher = "ciphering";

    deroule.add_section(sect_entrep, "Archive repository");
    deroule.add_section(sect_general, "General archive isolation options");
    deroule.add_section(sect_show, "What to show during the operation");
    deroule.add_section(sect_compr, "Compression options");
    deroule.add_section(sect_slice, "Slicing options");
    deroule.add_section(sect_cipher, "Encryption options");

    deroule.adopt_in_section(sect_entrep, &entrep);

    fs_archgen.adopt(&allow_over);
    fs_archgen.adopt(&warn_over);
    fs_archgen.adopt(&pause);
    fs_archgen.adopt(&slice_permission);
    fs_archgen.adopt(&slice_user_ownership);
    fs_archgen.adopt(&slice_group_ownership);
    fs_archgen.adopt(&sequential_marks);
    fs_archgen.adopt(&user_comment);
    fs_archgen.adopt(&slice_min_digits);
    fs_archgen.adopt(&hash_algo);
    fs_archgen.adopt(&execute);
    fs_archgen.adopt(&empty);
    form_archgen.adopt(&fs_archgen);
    deroule.adopt_in_section(sect_general, &form_archgen);

    fs_shown.adopt(&info_details);
    form_shown.adopt(&fs_shown);
    deroule.adopt_in_section(sect_show, &form_shown);

    fs_compr.adopt(&compression);
    fs_compr.adopt(&compression_level);
    form_compr.adopt(&fs_compr);
    deroule.adopt_in_section(sect_compr, &form_compr);

    fs_slicing.adopt(&slicing);
    fs_slicing.adopt(&slice_size);
    fs_slicing.adopt(&slice_size_unit);
    fs_slicing.adopt(&different_first_slice);
    fs_slicing.adopt(&first_slice_size);
    fs_slicing.adopt(&first_slice_size_unit);
    form_slicing.adopt(&fs_slicing);
    deroule.adopt_in_section(sect_slice, &form_slicing);

    fs_crypto.adopt(&crypto_algo);
    fs_crypto.adopt(&crypto_pass1);
    fs_crypto.adopt(&crypto_pass2);
    fs_crypto.adopt(&crypto_size);
    form_crypto.adopt(&fs_crypto);
    deroule.adopt_in_section(sect_cipher, &form_crypto);
    adopt(&deroule);

	// events and visibility
    compression.record_actor_on_event(this, html_compression::changed);
    slicing.record_actor_on_event(this, html_form_input::changed);
    different_first_slice.record_actor_on_event(this, html_form_input::changed);
    crypto_algo.record_actor_on_event(this, html_crypto_algo::changed);

    on_event("");

	// css
    webdar_css_style::grey_button(deroule, true);
}


string html_options_isolate::inherited_get_body_part(const chemin & path,
						     const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_options_isolate::on_event(const std::string & event_name)
{
    switch(compression.get_value())
    {
    case libdar::compression::none:
	compression_level.set_visible(false);
	break;
    case libdar::compression::gzip:
    case libdar::compression::bzip2:
    case libdar::compression::lzo:
	compression_level.set_visible(true);
	break;
    default:
	throw WEBDAR_BUG;
    }

    if(slicing.get_value_as_bool())
    {
	slice_size.set_visible(true);
	slice_size_unit.set_visible(true);
	different_first_slice.set_visible(true);
	if(different_first_slice.get_value_as_bool())
	{
	    first_slice_size.set_visible(true);
	    first_slice_size_unit.set_visible(true);
	}
	else
	{
	    first_slice_size.set_visible(false);
	    first_slice_size_unit.set_visible(false);
	}
    }
    else // no slicing requested
    {
	slice_size.set_visible(false);
	slice_size_unit.set_visible(false);
	different_first_slice.set_visible(false);
	first_slice_size.set_visible(false);
	first_slice_size_unit.set_visible(false);
    }

    switch(crypto_algo.get_value())
    {
    case libdar::crypto_algo::none:
	crypto_pass1.set_visible(false);
	crypto_pass2.set_visible(false);
	crypto_size.set_visible(false);
	break;
    case libdar::crypto_algo::scrambling:
    case libdar::crypto_algo::blowfish:
    case libdar::crypto_algo::aes256:
    case libdar::crypto_algo::twofish256:
    case libdar::crypto_algo::serpent256:
    case libdar::crypto_algo::camellia256:
	crypto_pass1.set_visible(true);
	crypto_pass2.set_visible(true);
	crypto_size.set_visible(true);
	break;
    default:
	throw WEBDAR_BUG;
    }

    my_body_part_has_changed();
}


libdar::archive_options_isolate html_options_isolate::get_options(std::shared_ptr<html_web_user_interaction> & webui) const
{
    libdar::archive_options_isolate ret;

    ret.set_entrepot(entrep.get_entrepot(webui));
    ret.set_allow_over(allow_over.get_value_as_bool());
    ret.set_warn_over(warn_over.get_value_as_bool());
    ret.set_pause(libdar::deci(pause.get_value()).computer());
    ret.set_slice_permission(slice_permission.get_value());
    ret.set_slice_user_ownership(slice_user_ownership.get_value());
    ret.set_slice_group_ownership(slice_group_ownership.get_value());
    ret.set_sequential_marks(sequential_marks.get_value_as_bool());
    ret.set_user_comment(user_comment.get_value());
    ret.set_slice_min_digits(libdar::deci(slice_min_digits.get_value()).computer());
    ret.set_hash_algo(hash_algo.get_value());
    ret.set_empty(empty.get_value_as_bool());
    ret.set_execute(execute.get_value());
    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_compression(compression.get_value());
    ret.set_compression_level(webdar_tools_convert_to_int(compression_level.get_value()));

    if(slicing.get_value_as_bool())
    {
	libdar::infinint s_size = libdar::deci(slice_size.get_value()).computer() * slice_size_unit.get_value();
	libdar::infinint f_s_size = 0;

	if(different_first_slice.get_value_as_bool())
	    f_s_size = libdar::deci(first_slice_size.get_value()).computer() * first_slice_size_unit.get_value();

	ret.set_slicing(s_size, f_s_size);
    }

    ret.set_crypto_algo(crypto_algo.get_value());
    if(crypto_algo.get_value() != libdar::crypto_algo::none)
    {
	if(crypto_pass1.get_value() != crypto_pass2.get_value())
	    throw exception_range("crypto password and its confirmation do not match");
	ret.set_crypto_pass(libdar::secu_string(crypto_pass1.get_value().c_str(), crypto_pass1.get_value().size()));
	ret.set_crypto_size(webdar_tools_convert_to_int(crypto_size.get_value()));
    }


    return ret;
}

void html_options_isolate::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}
