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


    // webdar headers
#include "webdar_tools.hpp"
#include "webdar_css_style.hpp"


    //
#include "html_options_read.hpp"

using namespace std;

html_options_read::html_options_read():
    form_src("Update Options"),
    fs_src("Archive Options"),
    fs_ref("External catalog Options"),
    src_crypto_algo("Cipher (for recent archives/backups, this is autodetected)"),
    src_crypto_pass("Passphrase", html_form_input::password, "", 30),
    src_crypto_size("Cipher block size", html_form_input::number, "0", 8),
    src_execute("Command to execute locally before reading each slice", html_form_input::text, "", 30),
    src_slice_min_digits("Slice minimum digit", html_form_input::number, "0", 8),
    info_details("Detailed information", html_form_input::check, "", 1),
    lax("Laxist check mode", html_form_input::check, "", 1),
    sequential_read("Sequential read", html_form_input::check, "", 1),
    ref_use_external_catalogue("Use external catalog to open the archive", html_form_input::check, "", 1),
    form_ref("Update Options"),
    ref_path("External catalog path", "/", 50, "Select the external catalog..."),
    ref_crypto_algo("Cipher"),
    ref_crypto_pass("Passphrase", html_form_input::password, "", 30),
    ref_crypto_size("Cipher block size", html_form_input::number, "0", 8),
    ref_execute("Command to execute locally before reading each slice", html_form_input::text, "", 30),
    ref_slice_min_digits("Slice minimum digit", html_form_input::number, "0", 8)
{
    ref_path.set_select_mode(html_form_input_file::select_slice);
    ref_path.set_can_create_dir(false);

	// set default values from libdar

    libdar::archive_options_read defaults;

    src_crypto_algo.set_value(defaults.get_crypto_algo());
    src_crypto_pass.set_value("");
    src_crypto_size.set_value(webdar_tools_convert_to_string(defaults.get_crypto_size()));
    src_execute.set_value(defaults.get_execute());
    src_slice_min_digits.set_value(webdar_tools_convert_to_string(defaults.get_slice_min_digits()));
    info_details.set_value_as_bool(defaults.get_info_details());
    lax.set_value_as_bool(defaults.get_lax());
    sequential_read.set_value_as_bool(defaults.get_sequential_read());
    ref_use_external_catalogue.set_value_as_bool(defaults.is_external_catalogue_set());
    if(ref_use_external_catalogue.get_value_as_bool())
	ref_path.set_value(defaults.get_ref_path().display());
    ref_crypto_algo.set_value(defaults.get_ref_crypto_algo());
    ref_crypto_pass.set_value("");
    ref_crypto_size.set_value(webdar_tools_convert_to_string(defaults.get_ref_crypto_size()));
    ref_execute.set_value(defaults.get_ref_execute());
    ref_slice_min_digits.set_value(webdar_tools_convert_to_string(defaults.get_ref_slice_min_digits()));

	// build the adoption tree
    static const char* sect_opt = "options";
    static const char* sect_extcat = "externalcat";
    deroule.add_section(sect_opt, "Reading options");
    deroule.add_section(sect_extcat, "External Catalog");

    fs_src.adopt(&src_crypto_algo);
    fs_src.adopt(&src_crypto_pass);
    fs_src.adopt(&src_crypto_size);
    fs_src.adopt(&src_execute);
    fs_src.adopt(&src_slice_min_digits);
    fs_src.adopt(&info_details);
    fs_src.adopt(&lax);
    fs_src.adopt(&sequential_read);
    form_src.adopt(&fs_src);
    deroule.adopt_in_section(sect_opt, &form_src);

    fs_ref.adopt(&ref_use_external_catalogue);
    fs_ref.adopt(&ref_path);
    fs_ref.adopt(&ref_crypto_algo);
    fs_ref.adopt(&ref_crypto_pass);
    fs_ref.adopt(&ref_crypto_size);
    fs_ref.adopt(&ref_execute);
    fs_ref.adopt(&ref_slice_min_digits);
    form_ref.adopt(&fs_ref);
    deroule.adopt_in_section(sect_extcat, &form_ref);

    adopt(&deroule);

	// binding the events

    src_crypto_algo.record_actor_on_event(this, html_crypto_algo::changed);
    ref_use_external_catalogue.record_actor_on_event(this, html_form_input::changed);
    ref_crypto_algo.record_actor_on_event(this, html_crypto_algo::changed);

	// manually launching on event to have coherent visibility between fields
    on_event(html_crypto_algo::changed);

	// css

    webdar_css_style::grey_button(deroule, true);
}

const libdar::archive_options_read & html_options_read::get_options() const
{
    html_options_read *me = const_cast <html_options_read *>(this);

    if(me == nullptr)
	throw WEBDAR_BUG;

    me->opts.clear();
    me->opts.set_crypto_algo(src_crypto_algo.get_value());
    if(src_crypto_algo.get_value() != libdar::crypto_algo::none)
    {
	me->opts.set_crypto_pass(libdar::secu_string(src_crypto_pass.get_value().c_str(), src_crypto_pass.get_value().size()));
	me->opts.set_crypto_size(webdar_tools_convert_to_int(src_crypto_size.get_value()));
    }
    me->opts.set_execute(src_execute.get_value());
    me->opts.set_slice_min_digits(libdar::infinint(webdar_tools_convert_to_int(src_slice_min_digits.get_value())));
    me->opts.set_info_details(info_details.get_value_as_bool());
    me->opts.set_lax(lax.get_value_as_bool());
    me->opts.set_sequential_read(sequential_read.get_value_as_bool());
    if(ref_use_external_catalogue.get_value_as_bool())
    {
	chemin chem(ref_path.get_value());

	if(chem.size() > 1)
	{
	    string basename = chem.back();
	    chem.pop_back();

	    me->opts.set_external_catalogue(chem.display(), basename);
	}
	else
	    me->opts.set_external_catalogue(string(""), chem.display());

	me->opts.set_ref_crypto_algo(ref_crypto_algo.get_value());
	if(ref_crypto_algo.get_value() != libdar::crypto_algo::none)
	{
	    me->opts.set_ref_crypto_pass(libdar::secu_string(ref_crypto_pass.get_value().c_str(), ref_crypto_pass.get_value().size()));
	    me->opts.set_ref_crypto_size(webdar_tools_convert_to_int(ref_crypto_size.get_value()));
	}
	me->opts.set_ref_execute(ref_execute.get_value());
	me->opts.set_ref_slice_min_digits(libdar::infinint(webdar_tools_convert_to_int(ref_slice_min_digits.get_value())));
    }
    else
	me->opts.unset_external_catalogue();

    return opts;
}

void html_options_read::on_event(const std::string & event_name)
{
    if(src_crypto_algo.get_value() == libdar::crypto_algo::none)
    {
	src_crypto_pass.set_visible(false);
	src_crypto_size.set_visible(false);
    }
    else
    {
	src_crypto_pass.set_visible(true);
	src_crypto_size.set_visible(true);
    }

    if(ref_crypto_algo.get_value() == libdar::crypto_algo::none)
    {
	ref_crypto_pass.set_visible(false);
	ref_crypto_size.set_visible(false);
    }
    else
    {
	ref_crypto_pass.set_visible(true);
	ref_crypto_size.set_visible(true);
    }

    if(ref_use_external_catalogue.get_value_as_bool())
    {
	ref_path.set_visible(true);
	ref_crypto_algo.set_visible(true);
	ref_execute.set_visible(true);
	ref_slice_min_digits.set_visible(true);
    }
    else
    {
	ref_path.set_visible(false);
	ref_crypto_algo.set_visible(false);
	ref_execute.set_visible(false);
	ref_slice_min_digits.set_visible(false);
	ref_crypto_pass.set_visible(false);
	ref_crypto_size.set_visible(false);

    }
}

string html_options_read::inherited_get_body_part(const chemin & path,
						  const request & req)
{
    string ret;

    if(get_next_visible() || get_visible())
	ret = get_body_part_from_all_children(path, req);

    if(!get_next_visible())
	ret = "";

    ack_visible();

    return ret;
}


void html_options_read::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}
