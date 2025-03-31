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
#include "webdar_css_style.hpp"
#include "tokens.hpp"

    //
#include "html_summary_page.hpp"

using namespace std;

const string html_summary_page::event_close = "html_summary_page::close";

html_summary_page::html_summary_page():
    format_table(2),
    slice_table(2),
    overall_table(2),
    content_table(2),
    saved_table(2),
    close("Close", event_close)
{

	// components setup
    set_session_name("");

    format_table.css_border_collapsed(true);
    slice_table.css_border_collapsed(true);
    overall_table.css_border_collapsed(true);
    content_table.css_border_collapsed(true);
    saved_table.css_border_collapsed(true);

	// the body_builder tree

    about.adopt(&about_text);
    adopt(&about);

    format_float.adopt(&format_title);
    format_float.adopt(&format_table);
    adopt(&format_float);

    slice_float.adopt(&slice_title);
    slice_float.adopt(&slice_table);
    adopt(&slice_float);

    overall_float.adopt(&overall_title);
    overall_float.adopt(&overall_table);
    adopt(&overall_float);

    content_float.adopt(&content_title);
    content_float.adopt(&content_table);
    adopt(&content_float);

    saved_float.adopt(&saved_title);
    saved_float.adopt(&saved_table);
    adopt(&saved_float);

    adopt(&close);

	// event binding
    register_name(event_close);
    close.record_actor_on_event(this, event_close);

	// css
    about.add_css_class(css_about_box);
    about_text.add_css_class(css_about_text);
    about_text.add_css_class(webdar_css_style::text_shadow_dark);

    format_title.add_css_class(webdar_css_style::text_shadow_dark);
    slice_title.add_css_class(webdar_css_style::text_shadow_dark);
    overall_title.add_css_class(webdar_css_style::text_shadow_dark);
    content_title.add_css_class(webdar_css_style::text_shadow_dark);
    saved_title.add_css_class(webdar_css_style::text_shadow_dark);

    format_table.set_css_class_cells(css_table_rest);
    slice_table.set_css_class_cells(css_table_rest);
    overall_table.set_css_class_cells(css_table_rest);
    content_table.set_css_class_cells(css_table_rest);
    saved_table.set_css_class_cells(css_table_rest);

    format_table.set_css_class_first_row(css_table_top);
    slice_table.set_css_class_first_row(css_table_top);
    overall_table.set_css_class_first_row(css_table_top);
    content_table.set_css_class_first_row(css_table_top);
    saved_table.set_css_class_first_row(css_table_top);

    content_table.set_css_class_first_column(css_table_left);
    format_table.set_css_class_first_column(css_table_left);
    slice_table.set_css_class_first_column(css_table_left);
    overall_table.set_css_class_first_column(css_table_left);
    saved_table.set_css_class_first_column (css_table_left);

    format_float.add_css_class(css_div_float);
    slice_float.add_css_class(css_div_float);
    overall_float.add_css_class(css_div_float);
    content_float.add_css_class(css_div_float);
    saved_float.add_css_class(css_div_float);

    webdar_css_style::normal_button(close, false);
    close.add_css_class(css_close);
}

void html_summary_page::on_event(const string & event_name)
{
    if(event_name != event_close)
	throw WEBDAR_BUG;
    act(event_close); // propagate the event
	// not calling 	my_body_part_has_changed();
	// as inherited_get_body_part() will not produce something
	// different due to this event
}

void html_summary_page::set_session_name(const string & session_name)
{
    string title = "Webdar - ";

    if(session_name != "")
	title += session_name + " - ";

    title += "Archive Summary";
    set_title(title);
}

void html_summary_page::set_source(const archive_init_list *ref)
{
    if(ref == nullptr)
	throw WEBDAR_BUG;

    if(! ref->opened())
	throw WEBDAR_BUG;

    about_text.clear();
    about_text.add_text(3, libdar::tools_printf("Properties of %s", ref->get_archive_full_name().c_str()));

    libdar::archive_summary sum = ref->get_summary();

    format_title.clear();
    format_title.add_text(2, "Format releated information");

    format_table.clear();
    format_table.adopt_static_html("Parameter");
    format_table.adopt_static_html("Value");
    format_table.adopt_static_html("Format version");
    format_table.adopt_static_html(sum.get_edition());
    format_table.adopt_static_html("Compression Algorithm");
    format_table.adopt_static_html(sum.get_compression_algo());
    format_table.adopt_static_html("User comment");
    format_table.adopt_static_html(sum.get_user_comment());
    format_table.adopt_static_html("Symmetrical encryption");
    format_table.adopt_static_html(sum.get_cipher());
    format_table.adopt_static_html("Asymmetricla encryption");
    format_table.adopt_static_html(sum.get_asym());
    format_table.adopt_static_html("Signed");
    format_table.adopt_static_html(sum.get_signed() ? "yes": "no");
    format_table.adopt_static_html("Tape marks");
    format_table.adopt_static_html(sum.get_tape_marks() ? "yes": "no");
    format_table.adopt_static_html("In place path");
    format_table.adopt_static_html(sum.get_in_place());

    slice_title.clear();
    slice_title.add_text(2, "Slicing information");

    slice_table.clear();
    slice_table.adopt_static_html("Parameter");
    slice_table.adopt_static_html("Value");
    slice_table.adopt_static_html("Number of slices");
    slice_table.adopt_static_html(libdar::deci(sum.get_slice_number()).human());
    if(sum.get_slice_number() > 1 && sum.get_first_slice_size() != sum.get_slice_size())
    {
	slice_table.adopt_static_html("First slice size");
	slice_table.adopt_static_html(libdar::deci(sum.get_first_slice_size()).human() + " bytes");
    }

    if(sum.get_slice_number() > 2)
    {
	slice_table.adopt_static_html("Slice size");
	slice_table.adopt_static_html(libdar::deci(sum.get_slice_size()).human() + " bytes");
    }

    if(sum.get_slice_number() > 1)
	slice_table.adopt_static_html("Last size");
    else
	slice_table.adopt_static_html("Slice size");
    slice_table.adopt_static_html(libdar::deci(sum.get_last_slice_size()).human() + " bytes");


    if(sum.get_ref_first_slice_size() != sum.get_first_slice_size()
       || sum.get_ref_slice_size() != sum.get_slice_size())
    {
	if(sum.get_ref_first_slice_size() != sum.get_ref_slice_size())
	{
	    slice_table.adopt_static_html("Ref first slice size");
	    slice_table.adopt_static_html(libdar::deci(sum.get_ref_first_slice_size()).human() + " bytes");
	}
	slice_table.adopt_static_html("Ref slice size");
	slice_table.adopt_static_html(libdar::deci(sum.get_ref_slice_size()).human() + " bytes");
    }

    overall_title.clear();
    overall_title.add_text(2, "Global sizes");

    overall_table.clear();
    overall_table.adopt_static_html("Parameter");
    overall_table.adopt_static_html("Value");
    overall_table.adopt_static_html("Archive size");
    overall_table.adopt_static_html(libdar::deci(sum.get_archive_size()).human() + " bytes");
    overall_table.adopt_static_html("Catalog size");
    overall_table.adopt_static_html(libdar::deci(sum.get_catalog_size()).human() + " bytes");
    overall_table.adopt_static_html("Storage size");
    overall_table.adopt_static_html(libdar::deci(sum.get_storage_size()).human() + " bytes");
    overall_table.adopt_static_html("Data size");
    overall_table.adopt_static_html(libdar::deci(sum.get_data_size()).human() + " bytes");

    content_title.clear();
    content_title.add_text(2, "Contents distribution");

    libdar::entree_stats estat = sum.get_contents();

    content_table.clear();
    content_table.adopt_static_html("Parameter");
    content_table.adopt_static_html("Value");
    content_table.adopt_static_html("Number of recorded as deleted");
    content_table.adopt_static_html(libdar::deci(estat.num_x).human());
    content_table.adopt_static_html("Number of directory");
    content_table.adopt_static_html(libdar::deci(estat.num_d).human());
    content_table.adopt_static_html("Number of plain files");
    content_table.adopt_static_html(libdar::deci(estat.num_f).human());
    content_table.adopt_static_html("Number of char devices");
    content_table.adopt_static_html(libdar::deci(estat.num_c).human());
    content_table.adopt_static_html("Number of block devices");
    content_table.adopt_static_html(libdar::deci(estat.num_b).human());
    content_table.adopt_static_html("Number of named pipes");
    content_table.adopt_static_html(libdar::deci(estat.num_p).human());
    content_table.adopt_static_html("Number of Unix sockets");
    content_table.adopt_static_html(libdar::deci(estat.num_s).human());
    content_table.adopt_static_html("Number of symbolic links");
    content_table.adopt_static_html(libdar::deci(estat.num_l).human());
    content_table.adopt_static_html("Number of Door files");
    content_table.adopt_static_html(libdar::deci(estat.num_D).human());
    content_table.adopt_static_html("Number of hard linked inodes");
    content_table.adopt_static_html(libdar::deci(estat.num_hard_linked_inodes).human());
    content_table.adopt_static_html("Number of hard link entries");
    content_table.adopt_static_html(libdar::deci(estat.num_hard_link_entries).human());

    saved_title.clear();
    saved_title.add_text(2, "Saved status distribution");

    saved_table.clear();
    saved_table.adopt_static_html("Parameter");
    saved_table.adopt_static_html("Value");
    saved_table.adopt_static_html("Number of fully saved inodes");
    saved_table.adopt_static_html(libdar::deci(estat.saved).human());
    saved_table.adopt_static_html("Number of patched inodes");
    saved_table.adopt_static_html(libdar::deci(estat.patched).human());
    saved_table.adopt_static_html("Number of inodes with only metadata saved");
    saved_table.adopt_static_html(libdar::deci(estat.inode_only).human());
    saved_table.adopt_static_html("Total number of inodes");
    saved_table.adopt_static_html(libdar::deci(estat.total).human());
}

void html_summary_page::clear()
{
    format_title.clear();
    format_table.clear();
    slice_title.clear();
    slice_table.clear();
    overall_title.clear();
    overall_table.clear();
    content_title.clear();
    content_table.clear();
    saved_title.clear();
    saved_table.clear();
}


void html_summary_page::new_css_library_available()
{
    css_class page("html_summary_page");
    css tmp;
    unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    if(!csslib->class_exists(css_close))
    {
	tmp.clear();
	tmp.css_float(css::fl_right);
	tmp.css_float_clear(css::fc_both);
	tmp.css_position_type(css::pos_fixed);
	tmp.css_position_bottom("1em");
	tmp.css_position_right("1em");
	csslib->add(css_close, tmp);
    }

    if(!csslib->class_exists(css_table_top))
    {
	tmp = webdar_css_style::get_css_class(webdar_css_style::text_shadow_dark);

	tmp.css_border_width(css::bd_all, css::bd_medium);
	tmp.css_border_style(css::bd_all, css::bd_solid);
	tmp.css_font_weight_bold();
	tmp.css_background_color(COLOR_BACK);
	tmp.css_color(COLOR_TEXT);
	csslib->add(css_table_top, tmp);
    }

    if(!csslib->class_exists(css_table_left))
    {
	tmp.clear();
	tmp.css_border_width(css::bd_all, css::bd_medium);
	tmp.css_border_style(css::bd_all, css::bd_solid);
	tmp.css_text_h_align(css::al_left);
	tmp.css_font_weight_bold();
	csslib->add(css_table_left, tmp);
    }

    if(!csslib->class_exists(css_table_rest))
    {
	tmp.clear();
	tmp.css_border_width(css::bd_all, css::bd_medium);
	tmp.css_border_style(css::bd_all, css::bd_solid);
	tmp.css_text_h_align(css::al_right);
	csslib->add(css_table_rest, tmp);
    }

    if(!csslib->class_exists(css_div_float))
    {
	tmp.clear();
	tmp.css_float(css::fl_left);
	tmp.css_margin("1em");
	csslib->add(css_div_float, tmp);
    }

    if(!csslib->class_exists(css_about_box))
    {
	tmp.clear();
	tmp.css_overflow(css::ov_hidden);
	tmp.css_width("100%", true);
	tmp.css_background_color(COLOR_TOPBAR_BACK);
	csslib->add(css_about_box, tmp);
    }

    if(!csslib->class_exists(css_about_text))
    {
	tmp.clear();
	tmp.css_font_weight_bold();
	tmp.css_text_v_align(css::al_middle);
	tmp.css_margin_left("1em");
	tmp.css_color(COLOR_MENU_BORDER_OFF);
	csslib->add(css_about_text, tmp);
    }

    webdar_css_style::update_library(*csslib);
}
