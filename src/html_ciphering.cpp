/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2024 Denis Corbin
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
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/tools.hpp>

    // webdar headers
#include "webdar_tools.hpp"


    //
#include "html_ciphering.hpp"

using namespace std;

const string html_ciphering::changed = "html_ciphering_changed";


html_ciphering::html_ciphering():
    form_crypto("Update"),
    crypto_fs(""),
    crypto_type("type of cryptography"),
    crypto_algo("Cipher used"),
    crypto_pass1("Pass phrase", html_form_input::password, "", "80%"),
    crypto_pass2("Confirm pass phrase", html_form_input::password, "", "80%"),
    crypto_size("Cipher Block size", 0, "30"),
    crypto_threads("Number of threads for ciphering", html_form_input::number, "2", "5"),
    crypto_fs_kdf_hash("Key Derivation Function"),
    iteration_count("Iteration count", html_form_input::number, "1", "30"),
    ignore_events(false)

{
    libdar::archive_options_create defaults;

	// components configuration

	// the order in the following must match the html_ciphering::crypto_type_t enum
    crypto_type.add_choice("sym", "Symmetric encryption");
    crypto_type.add_choice("asym", "Asymmetric encryption");
    crypto_type.set_selected_id("sym");
    crypto_algo.set_value(defaults.get_crypto_algo());

    crypto_pass1.set_value("");
    crypto_pass2.set_value("");
    crypto_threads.set_min_only(1);
    crypto_threads.set_value_as_int(defaults.get_multi_threaded_crypto());
    crypto_kdf_hash.add_choice("md5","md5");
    crypto_kdf_hash.add_choice("sha1","sha1");
    crypto_kdf_hash.add_choice("sha512","sha512");
    crypto_kdf_hash.add_choice("whirlpool","whirlpool");
    crypto_kdf_hash.add_choice("argon2","argon2");
    crypto_kdf_hash.set_selected_id("argon2");
    crypto_size.set_value_as_infinint(defaults.get_crypto_size());
    iteration_count.set_value(libdar::deci(defaults.get_iteration_count()).human());


	// adoption tree

    crypto_fs.adopt(&crypto_algo);
    crypto_fs.adopt(&crypto_type);
    crypto_fs.adopt(&crypto_pass1);
    crypto_fs.adopt(&crypto_pass2);
    crypto_fs.adopt(&gnupg);
    crypto_fs.adopt(&crypto_size);
    crypto_fs.adopt(&crypto_threads);
    form_crypto.adopt(&crypto_fs);
    crypto_fs_kdf_hash.adopt(&crypto_kdf_hash);
    crypto_fs_kdf_hash.adopt(&iteration_count);
    form_crypto.adopt(&crypto_fs_kdf_hash);
    adopt(&form_crypto);

	// events

    crypto_algo.record_actor_on_event(this, html_crypto_algo::changed);
    crypto_type.record_actor_on_event(this, html_form_select::changed);
    crypto_pass1.record_actor_on_event(this, html_form_input::changed);
    crypto_pass2.record_actor_on_event(this, html_form_input::changed);
    crypto_size.record_actor_on_event(this, html_form_input_unit::changed);
    gnupg.record_actor_on_event(this, html_form_gnupg_list::changed);
    crypto_kdf_hash.set_change_event_name(kdf_algo_changed);
    crypto_kdf_hash.record_actor_on_event(this, kdf_algo_changed);
    iteration_count.record_actor_on_event(this, html_form_input::changed);
    register_name(changed);

	// setup visibility

    on_event(html_form_select::changed);
    on_event(kdf_algo_changed);

	// csss

	// nothing //
}

void html_ciphering::set_crypto_size_range(const libdar::infinint & min,
					   const libdar::infinint & max)
{
    crypto_size.set_range(min, max);
}


libdar::U_I html_ciphering::get_multi_threaded_crypto() const
{
    return webdar_tools_convert_to_int(crypto_threads.get_value());
}

libdar::crypto_algo html_ciphering::get_crypto_algo() const
{
    return crypto_algo.get_value();
}

html_ciphering::crypto_type_t html_ciphering::get_crypto_type() const
{
    return crypto_type_t(crypto_type.get_selected_num());
}

libdar::secu_string html_ciphering::get_crypto_pass() const
{
    if(get_crypto_type() != sym)
	throw WEBDAR_BUG;

    if(crypto_pass1.get_value() != crypto_pass2.get_value())
	throw exception_range("crypto password and its confirmation do not match");

    return libdar::secu_string(crypto_pass1.get_value().c_str(), crypto_pass1.get_value().size());
}


libdar::infinint html_ciphering::get_iteration_count() const
{
    return libdar::deci(iteration_count.get_value()).computer();
}

libdar::hash_algo html_ciphering::get_kdf_hash() const
{
    switch(crypto_kdf_hash.get_selected_num())
    {
    case 0:
	return libdar::hash_algo::md5;
    case 1:
	return libdar::hash_algo::sha1;
    case 2:
	return libdar::hash_algo::sha512;
    case 3:
	return libdar::hash_algo::whirlpool;
    case 4:
	return libdar::hash_algo::argon2;
    default:
	throw WEBDAR_BUG;
    }
}

vector<string> html_ciphering::get_gnupg_recipients() const
{
    return gnupg.get_gnupg_recipients();
}

vector<string> html_ciphering::get_gnupg_signatories() const
{
    return gnupg.get_gnupg_signatories();
}

libdar::U_32 html_ciphering::get_crypto_size() const
{
    return webdar_tools_convert_from_infinint<libdar::U_32>(
	crypto_size.get_value_as_infinint(),
	string("Value too large for a cipher block size"));
}

void html_ciphering::load_json(const json & source)
{
    try
    {
	unsigned int version;
	string class_id;
	json config = unwrap_config_from_json_header(source,
						     version,
						     class_id);

	if(class_id != myclass_id)
	    throw exception_range(libdar::tools_printf("Unexpected class_id in json data, found %s while expecting %s",
						       class_id.c_str(),
						       myclass_id));

	if(version > format_version)
	    throw exception_range(libdar::tools_printf("Json format version too hight for %s, upgrade your webdar software", myclass_id));

	ignore_events = true;

	try
	{
	    crypto_type.set_selected_id_with_warning(config.at(jlabel_type), jlabel_type);
	    crypto_algo.set_selected_id_with_warning(config.at(jlabel_algo), jlabel_algo);
	    crypto_pass1.set_value(config.at(jlabel_pass));
	    crypto_pass2.set_value(config.at(jlabel_pass));
	    crypto_size.set_value_as_infinint(libdar::deci(config.at(jlabel_size)).computer());
	    crypto_threads.set_value_as_int(config.at(jlabel_threads));
	    gnupg.load_json(config.at(jlabel_gnupg));
	    crypto_kdf_hash.set_selected_id_with_warning(config.at(jlabel_kdf_hash), jlabel_kdf_hash);
	    iteration_count.set_value_as_int(config.at(jlabel_kdf_iter));
	}
	catch(...)
	{
	    ignore_events = false;
	    throw;
	}
	ignore_events = false;

	on_event(html_form_select::changed);
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }

}

json html_ciphering::save_json() const
{
    json ret;

    ret[jlabel_type] = crypto_type.get_selected_id();
    ret[jlabel_algo] = crypto_algo.get_selected_id();
    ret[jlabel_pass] = crypto_pass1.get_value(); // assuming pass2 is equal to pass1
    ret[jlabel_size] = libdar::deci(crypto_size.get_value_as_infinint()).human();
    ret[jlabel_threads] = crypto_threads.get_value_as_int();
    ret[jlabel_gnupg] = gnupg.save_json();
    ret[jlabel_kdf_hash] = crypto_kdf_hash.get_selected_id();
    ret[jlabel_kdf_iter] = iteration_count.get_value_as_int();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					ret);
}

void html_ciphering::clear_json()
{
    libdar::archive_options_create defaults;

    crypto_type.set_selected_num(0);
    crypto_algo.set_selected_num(0);
    crypto_pass1.set_value("");
    crypto_pass2.set_value("");
    crypto_size.set_value_as_infinint(defaults.get_crypto_size());
    crypto_threads.set_value_as_int(defaults.get_multi_threaded_crypto());
    gnupg.clear_json();
    set_kdf_hash(defaults.get_kdf_hash());
    iteration_count.set_value(libdar::deci(defaults.get_iteration_count()).human());
}

void html_ciphering::on_event(const string & event_name)
{
    if(ignore_events)
	return;

    if(event_name == html_crypto_algo::changed
       || event_name == html_form_select::changed)
    {
	switch(crypto_algo.get_value())
	{
	case libdar::crypto_algo::none:
	    crypto_type.set_visible(false);
	    crypto_pass1.set_visible(false);
	    crypto_pass2.set_visible(false);
	    crypto_size.set_visible(false);
	    crypto_threads.set_visible(false);
	    gnupg.set_visible(false);
	    crypto_fs_kdf_hash.set_visible(false);
	    break;
	case libdar::crypto_algo::scrambling:
	case libdar::crypto_algo::blowfish:
	case libdar::crypto_algo::aes256:
	case libdar::crypto_algo::twofish256:
	case libdar::crypto_algo::serpent256:
	case libdar::crypto_algo::camellia256:
	    crypto_type.set_visible(true);
	    switch(crypto_type_t(crypto_type.get_selected_num()))
	    {
	    case sym:
		crypto_pass1.set_visible(true);
		crypto_pass2.set_visible(true);
		gnupg.set_visible(false);
		crypto_fs_kdf_hash.set_visible(true);
		break;
	    case asym:
		crypto_pass1.set_visible(false);
		crypto_pass2.set_visible(false);
		gnupg.set_visible(true);
		crypto_fs_kdf_hash.set_visible(false);
		break;
	    default:
		throw WEBDAR_BUG;
	    }
	    crypto_size.set_visible(true);
	    crypto_threads.set_visible(true);
	    break;
	default:
	    throw WEBDAR_BUG;
	}
	act(changed);
    }
    else if(event_name == kdf_algo_changed)
    {
	libdar::infinint cur_it = libdar::deci(iteration_count.get_value()).computer();

	if(get_kdf_hash() != libdar::hash_algo::argon2)
	{
	    if(cur_it < libdar::default_iteration_count)
		iteration_count.set_value(libdar::deci(libdar::default_iteration_count).human());
	    iteration_count.set_min_only(
		webdar_tools_convert_from_infinint<int>(
		    libdar::default_iteration_count,
		    string("too large integer default value provided by libdar")
		    )
		);
	}
	else
	{
	    if(cur_it < libdar::default_iteration_count_argon2)
		iteration_count.set_value(libdar::deci(libdar::default_iteration_count_argon2).human());
	    iteration_count.set_min_only(
		webdar_tools_convert_from_infinint<int>(
		    libdar::default_iteration_count_argon2,
		    string("too large integer default value provided by libdar")));

	    if(cur_it == libdar::default_iteration_count)
		iteration_count.set_value(libdar::deci(libdar::default_iteration_count_argon2).human());
		// we will not change back to lower value when swapping back to argon2
		// unless the value is the default one (thus user can change the hash
		// algorithm and stick to the default value for the selected hash algo).
	}
	act(changed);
    }
    else if(event_name == html_form_input::changed
	    || event_name == html_form_input_unit::changed
	    || event_name == html_form_gnupg_list::changed)
	act(changed);
    else
	throw WEBDAR_BUG;
}

void html_ciphering::set_to_webdar_defaults()
{
    clear_json();

    crypto_algo.set_value(libdar::crypto_algo::aes256);
    crypto_type.set_selected_num(0); // should be symmetrical encryption
    iteration_count.set_value_as_int(20000); // default algo is argon2
}

string html_ciphering::inherited_get_body_part(const chemin & path,
					       const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_ciphering::css_classes_have_changed()
{
    set<string> css_classes = get_css_classes_as_a_set();

    form_crypto.clear_css_classes();
    for(set<string>::iterator it = css_classes.begin();
	it != css_classes.end();
	++it)
	form_crypto.add_css_class(*it);
}

void html_ciphering::set_kdf_hash(libdar::hash_algo hash)
{
    switch(hash)
    {
    case libdar::hash_algo::md5:
	crypto_kdf_hash.set_selected_num(0);
	break;
    case libdar::hash_algo::sha1:
	crypto_kdf_hash.set_selected_num(1);
	break;
    case libdar::hash_algo::sha512:
	crypto_kdf_hash.set_selected_num(2);
	break;
    case libdar::hash_algo::whirlpool:
	crypto_kdf_hash.set_selected_num(3);
	break;
    case libdar::hash_algo::argon2:
	crypto_kdf_hash.set_selected_num(4);
	break;
    default:
	throw WEBDAR_BUG;
    }
}
