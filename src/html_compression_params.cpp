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


    // webdar headers



    //
#include "html_compression_params.hpp"

using namespace std;

const string html_compression_params::changed = "html_compression_params_changed";


html_compression_params::html_compression_params(bool show_resave,
						 bool show_min_size,
						 bool show_keep_compressed):
    form_compr("Update"),
    compr_fs("Compression parameters"),
    compression("Compression algorithm"),
    compression_level("Compression level", html_form_input::number, "", "5"),
    min_compr_size("Minimum file sized compressed", 0, "30"),
    compression_block("Block compression for parallel compression (zero to zero to disable)", 0, "30"),
    never_resave_uncompressed("Never resave uncompressed if compressed file took more place than uncompressed", html_form_input::check, "", "1"),
    keep_compressed("Keep file compressed", html_form_input::check, "", "1"),
    compr_threads("Number of threads for compression", html_form_input::number, "2", "5"),
    x_show_resave(show_resave),
    x_show_min_size(show_min_size)
{

	// component setup
    compr_threads.set_min_only(1);
    compression.set_no_CR();
    min_compr_size.set_min_only(0);

	// visibility
    if(! x_show_resave)
	never_resave_uncompressed.set_visible(false);
    if(! x_show_min_size)
	min_compr_size.set_visible(false);
    if(! show_keep_compressed)
	keep_compressed.set_visible(false);

	// adoption tree
    compr_fs.adopt(&keep_compressed);
    compr_fs.adopt(&compression);
    compr_fs.adopt(&compression_level);
    compr_fs.adopt(&min_compr_size);
    compr_fs.adopt(&compression_block);
    compr_fs.adopt(&never_resave_uncompressed);
    compr_fs.adopt(&compr_threads);
    form_compr.adopt(&compr_fs);
    adopt(&form_compr);

	// events
    register_name(changed);
    compression.record_actor_on_event(this, html_compression::changed);
    keep_compressed.record_actor_on_event(this, html_form_input::changed);


	// css
    on_event(html_compression::changed);

}

void html_compression_params::on_event(const string & event_name)
{
    if(event_name == html_compression::changed
       || event_name == html_form_input::changed)
    {
	if(keep_compressed.get_value_as_bool())
	{
	    compression.set_visible(false);
	    compression_level.set_visible(false);
	    min_compr_size.set_visible(false);
	    compression_block.set_visible(false);
	    never_resave_uncompressed.set_visible(false);
	    compr_threads.set_visible(false);
	}
	else
	{
	    compression.set_visible(true);

	    switch(compression.get_value())
	    {
	    case libdar::compression::none:
		compression_level.set_visible(false);
		compression.set_no_CR(false);
		if(x_show_min_size)
		    min_compr_size.set_visible(false);
		compression_block.set_visible(false);
		if(x_show_resave)
		    never_resave_uncompressed.set_visible(false);
		compr_threads.set_visible(false);
		break;
	    case libdar::compression::lzo1x_1_15:
	    case libdar::compression::lzo1x_1:
		compression.set_no_CR(false);
		compression_level.set_visible(false);
		if(x_show_min_size)
		    min_compr_size.set_visible(true);
		compression_block.set_visible(true);
		if(x_show_resave)
		    never_resave_uncompressed.set_visible(true);
		compr_threads.set_visible(true);
		break;
	    case libdar::compression::gzip:
	    case libdar::compression::bzip2:
	    case libdar::compression::lzo:
	    case libdar::compression::xz:
	    case libdar::compression::zstd:
	    case libdar::compression::lz4:
		compression.set_no_CR(true);
		compression_level.set_visible(true);
		if(compression.get_value() == libdar::compression::zstd)
		    compression_level.set_range(1, 22);
		else
		    compression_level.set_range(1, 9);
		if(x_show_min_size)
		    min_compr_size.set_visible(true);
		compression_block.set_visible(true);
		if(x_show_resave)
		    never_resave_uncompressed.set_visible(true);
		compr_threads.set_visible(true);
		break;
	    default:
		throw WEBDAR_BUG;
	    }
	}

	act(changed);
    }
    else
	throw WEBDAR_BUG;
}

string html_compression_params::inherited_get_body_part(const chemin & path,
							const request & req)
{
    return get_body_part_from_all_children(path, req);
}
