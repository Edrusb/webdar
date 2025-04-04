/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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
#include "webdar_css_style.hpp"


    //
#include "html_statistics.hpp"

using namespace std;

html_statistics::html_statistics()
{
    table = nullptr;

	// css

    treated_lbl.add_css_class(webdar_css_style::text_right);
    hard_links_lbl.add_css_class(webdar_css_style::text_right);
    skipped_lbl.add_css_class(webdar_css_style::text_right);
    ignored_lbl.add_css_class(webdar_css_style::text_right);
    tooold_lbl.add_css_class(webdar_css_style::text_right);
    errored_lbl.add_css_class(webdar_css_style::text_right);
    deleted_lbl.add_css_class(webdar_css_style::text_right);
    ea_treated_lbl.add_css_class(webdar_css_style::text_right);
    byte_amount_lbl.add_css_class(webdar_css_style::text_right);
    total_lbl.add_css_class(webdar_css_style::text_right);

    total_lbl.add_css_class(webdar_css_style::text_bold);

    treated_count.add_css_class(webdar_css_style::text_bold);
    hard_links_count.add_css_class(webdar_css_style::text_bold);
    skipped_count.add_css_class(webdar_css_style::text_bold);
    ignored_count.add_css_class(webdar_css_style::text_bold);
    tooold_count.add_css_class(webdar_css_style::text_bold);
    errored_count.add_css_class(webdar_css_style::text_bold);
    deleted_count.add_css_class(webdar_css_style::text_bold);
    ea_treated_count.add_css_class(webdar_css_style::text_bold);
    byte_amount_count.add_css_class(webdar_css_style::text_bold);
    total_count.add_css_class(webdar_css_style::text_bold);
}

void html_statistics::clear_counters()
{
    stats.clear();
    update_html_counters();
}


void html_statistics::clear_labels()
{
    treated_lbl.clear();
    hard_links_lbl.clear();
    skipped_lbl.clear();
    ignored_lbl.clear();
    tooold_lbl.clear();
    errored_lbl.clear();
    deleted_lbl.clear();
    ea_treated_lbl.clear();
    byte_amount_lbl.clear();
    total_lbl.clear();
    unbuild();
    ignore_body_changed_from_my_children(true);
}

string html_statistics::inherited_get_body_part(const chemin & path,
						const request & req)
{
    update_html_counters();

    if(table == nullptr)
	build();

    return get_body_part_from_all_children(path, req);
}

void html_statistics::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;
    webdar_css_style::update_library(*csslib);
}

void html_statistics::build()
{
    if(table != nullptr)
	throw WEBDAR_BUG; // table is already built
    table = new (nothrow) html_table(2);
    if(table == nullptr)
	throw WEBDAR_BUG;
    try
    {
	if(treated_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(treated_lbl.get_body_part());
	    table->adopt(&treated_count);
	}
	if(hard_links_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(hard_links_lbl.get_body_part());
	    table->adopt(&hard_links_count);
	}
	if(skipped_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(skipped_lbl.get_body_part());
	    table->adopt(&skipped_count);
	}
	if(ignored_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(ignored_lbl.get_body_part());
	    table->adopt(&ignored_count);
	}
	if(tooold_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(tooold_lbl.get_body_part());
	    table->adopt(&tooold_count);
	}
	if(errored_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(errored_lbl.get_body_part());
	    table->adopt(&errored_count);
	}
	if(deleted_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(deleted_lbl.get_body_part());
	    table->adopt(&deleted_count);
	}
	if(ea_treated_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(ea_treated_lbl.get_body_part());
	    table->adopt(&ea_treated_count);
	}
	if(byte_amount_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(byte_amount_lbl.get_body_part());
	    table->adopt(&byte_amount_count);
	}
	if(total_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(total_lbl.get_body_part());
	    table->adopt(&total_count);
	}
	adopt(table);
    }
    catch(...)
    {
	if(table != nullptr)
	{
	    delete table;
	    table = nullptr;
	}
	throw;
    }
}

void html_statistics::unbuild()
{
    if(table != nullptr)
    {
	delete table;
	table = nullptr;
    }
}


void html_statistics::update_html_counters()
{
    if(treated_lbl.get_body_part() != "")
    {
	treated_count.clear();
	treated_count.add_text(0, libdar::deci(stats.get_treated()).human());
    }
    if(hard_links_lbl.get_body_part() != "")
    {
	hard_links_count.clear();
	hard_links_count.add_text(0, libdar::deci(stats.get_hard_links()).human());
    }
    if(skipped_lbl.get_body_part() != "")
    {
	skipped_count.clear();
	skipped_count.add_text(0, libdar::deci(stats.get_skipped()).human());
    }
    if(ignored_lbl.get_body_part() != "")
    {
	ignored_count.clear();
	ignored_count.add_text(0, libdar::deci(stats.get_ignored()).human());
    }
    if(tooold_lbl.get_body_part() != "")
    {
	tooold_count.clear();
	tooold_count.add_text(0, libdar::deci(stats.get_tooold()).human());
    }
    if(errored_lbl.get_body_part() != "")
    {
	errored_count.clear();
	errored_count.add_text(0, libdar::deci(stats.get_errored()).human());
    }
    if(deleted_lbl.get_body_part() != "")
    {
	deleted_count.clear();
	deleted_count.add_text(0, libdar::deci(stats.get_deleted()).human());
    }
    if(ea_treated_lbl.get_body_part() != "")
    {
	ea_treated_count.clear();
	ea_treated_count.add_text(0, libdar::deci(stats.get_ea_treated()).human());
    }
    if(byte_amount_lbl.get_body_part() != "")
    {
	byte_amount_count.clear();
	byte_amount_count.add_text(0, libdar::deci(stats.get_byte_amount()).human());
    }
    if(total_lbl.get_body_part() != "")
    {
	total_count.clear();
	total_count.add_text(0, libdar::deci(stats.total()).human());
    }
}
