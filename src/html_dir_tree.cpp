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
#include "tokens.hpp"
#include "html_table.hpp"
#include "html_text.hpp"
#include "webdar_css_style.hpp"

    //
#include "html_dir_tree.hpp"

using namespace std;

const string html_dir_tree::event_shrink = "html_dir_tree_shrink";
const string html_dir_tree::event_expand = "html_dir_tree_expand";
const string html_dir_tree::event_click = "html_dir_tree_click";

static constexpr const char* name_css_this = "hdt_this";
static constexpr const char* name_css_button = "html_dir_tree_button"; // applied to shrink/expand/nosubdir
static constexpr const char* name_css_name = "html_dir_tree_name";
static constexpr const char* name_css_forsubdirs = "html_dir_tree_forsubdir";
static constexpr const char* name_css_contents = "html_dir_tree_contents";
static constexpr const char* name_css_contents_title = "html_dir_tree_contents_title";
static constexpr const char* name_css_contents_cells = "html_dir_tree_contents_cells";

html_dir_tree::html_dir_tree(const string & chemin):
    shrink("-", event_shrink),
    expand("+", event_expand),
    nosubdir(" ",""),
    name("", event_click),
    contents(11)
{
    init(chemin);
}

html_dir_tree::html_dir_tree(const archive_init_list * ref, const string & chemin):
    shrink("-", event_shrink),
    expand("+", event_expand),
    nosubdir(" ",""),
    name("", event_click),
    contents(11)
{
    init(chemin);
    set_source(ref);
}

void html_dir_tree::init(const string & chemin)
{
    my_path = chemin;
    src = nullptr;
    info_read = false;
    has_sub = true; // until checked whether it has or not
    visibility_has_changed = false;
    focus_place = nullptr;
    focus_title = nullptr;

    if(chemin == "")
	name.change_label("&lt;ROOT&gt;");
    else
	name.change_label(my_path.back());

	// The body_builder tree

    line.adopt(&nosubdir);
    line.adopt(&shrink);
    line.adopt(&expand);
    line.adopt(&name);
    adopt(&line);
    adopt(&for_subdirs);

	// binding to events
    shrink.record_actor_on_event(this, event_shrink);
    expand.record_actor_on_event(this, event_expand);
    name.record_actor_on_event(this, event_click);

	// css
    add_css_class(name_css_this);

    webdar_css_style::small_button(shrink);
    webdar_css_style::small_button(expand);
    webdar_css_style::small_button(nosubdir);

    nosubdir.add_css_class(webdar_css_style::float_left);
    shrink.add_css_class(webdar_css_style::float_left);
    expand.add_css_class(webdar_css_style::float_left);
    line.add_css_class(webdar_css_style::float_left);

    name.add_css_class(webdar_css_style::float_left);
    name.url_add_css_class(name_css_name);
    name.add_css_class(webdar_css_style::text_shadow_dark);

    nosubdir.add_css_class(name_css_button);
    shrink.add_css_class(name_css_button);
    expand.add_css_class(name_css_button);

    for_subdirs.add_css_class(name_css_forsubdirs);

    contents.add_css_class(name_css_contents);
    contents.set_css_class_cells(name_css_contents_cells);
    contents.set_css_class_first_row(name_css_contents_title);

	// set initial visibility of objects
    on_event(event_shrink);

	// "contents" is not directly adopted and is filled with go_init_indent()
}

void html_dir_tree::clear()
{
    vector<html_dir_tree *>::iterator it = subdirs.begin();

    while(it != subdirs.end())
    {
	if(*it != nullptr)
	{
	    delete *it;
	    *it = nullptr;
	}
	++it;
    }
    subdirs.clear();
    src = nullptr;
    info_read = false;
    has_sub = true;
    my_body_part_has_changed();
    clear_contents();
}

void html_dir_tree::set_source(const archive_init_list *ref)
{
    if(ref == nullptr)
	throw WEBDAR_BUG;
    src = ref;
    if(!src->has_subdirectory(my_path.display(true)))
	go_hide();
    else
    {
	nosubdir.set_visible(false);
	visibility_has_changed = true;
	my_body_part_has_changed();
    }
}

void html_dir_tree::go_shrink()
{
    if(has_sub)
    {
	visibility_has_changed = true;
	shrink.set_visible(false);
	expand.set_visible(true);
	for_subdirs.set_visible(false);
    }
}

void html_dir_tree::go_expand()
{
    if(has_sub)
    {
	visibility_has_changed = true;
	shrink.set_visible(true);
	expand.set_visible(false);
	go_init_indent();
	if(subdirs.size() > 0)
	    for_subdirs.set_visible(true);
    }
}

void html_dir_tree::go_hide()
{
    has_sub = false;
    shrink.set_visible(false);
    expand.set_visible(false);
    nosubdir.set_visible(true);
    for_subdirs.set_visible(false);
    visibility_has_changed = true;
}

void html_dir_tree::on_event(const string & event_name)
{
    if(event_name == event_shrink)
	go_shrink();
    else if(event_name == event_expand)
	go_expand();
    else if(event_name == event_click)
    {
	if(focus_place != nullptr)
	    focus_place->given_for_temporary_adoption(&contents);
	if(focus_title != nullptr)
	{
	    focus_title->clear_children();
	    focus_title->adopt_static_html(my_path.display(false));
	}
	go_init_indent();
    }
    else
	throw WEBDAR_BUG;
    my_body_part_has_changed();
}

string html_dir_tree::inherited_get_body_part(const chemin & path,
					      const request & req)
{
    string ret;

    do
    {
	visibility_has_changed = false;
	ret = html_div::inherited_get_body_part(path, req);
    }
    while(visibility_has_changed);

    return ret;
}

void html_dir_tree::go_init_indent()
{
    if(!info_read)
    {
	info_read = true;

	if(src == nullptr)
	    throw WEBDAR_BUG;

	vector<libdar::list_entry> tmp = src->get_children_in_table(my_path.display(true));
	vector<libdar::list_entry>::const_iterator it = tmp.begin();

	clear_contents();
	while(it != tmp.end())
	{
	    if(it->is_dir())
	    {
		html_dir_tree *tmp_sub = new (nothrow) html_dir_tree(src, (my_path + chemin(it->get_name())).display(true));

		if(tmp_sub == nullptr)
		    throw exception_memory();

		try
		{
		    if(focus_place != nullptr)
			tmp_sub->set_drop_content(focus_place);
		    if(focus_title != nullptr)
			tmp_sub->set_drop_path(focus_title);
		    subdirs.push_back(tmp_sub);
		}
		catch(...)
		{
		    delete tmp_sub;
		    throw;
		}

		for_subdirs.adopt(tmp_sub);
	    }
		// fill table contents too

		// filename
	    contents.adopt_static_html(it->get_name());

		// Data
	    if(it->has_data_present_in_the_archive())
		if(it->is_dirty())
		    contents.adopt_static_html("[Dirty]");
		else
		    contents.adopt_static_html("[Saved]");
	    else
		contents.adopt_static_html("[     ]");

		// delta signature
	    contents.adopt_static_html(it->get_delta_flag());

		// EA
	    if(it->has_EA())
		if(it->has_EA_saved_in_the_archive())
		    contents.adopt_static_html("[Saved]");
		else
		    contents.adopt_static_html("[     ]");
	    else
		contents.adopt_static_html("-------");

		// compr
	    contents.adopt_static_html(it->get_compression_ratio());

		// sparse
	    contents.adopt_static_html(it->is_sparse() ? "X" : " ");

		// permissions
	    contents.adopt_static_html(it->get_perm());

		// UID
	    contents.adopt_static_html(it->get_uid());

		// GID
	    contents.adopt_static_html(it->get_gid());

		// size
	    contents.adopt_static_html(it->get_file_size());

		// modification date
	    contents.adopt_static_html(it->get_last_modif());

	    ++it;
	}
    }
}

void html_dir_tree::new_css_library_available()
{

    if(is_css_class_defined_in_library(name_css_name))
    {
	if(! is_css_class_defined_in_library(name_css_forsubdirs)
	   || ! is_css_class_defined_in_library(name_css_contents)
	   || ! is_css_class_defined_in_library(name_css_contents_cells)
	    )
	    throw WEBDAR_BUG;
    }
    else
    {
	css tmpcss;
	css_class tmpclass(name_css_name);

	    // property for the directory name/link in the directory tree

	tmpcss.css_box_sizing(css::bx_border);
	tmpcss.css_text_decoration(css::dc_none);
	tmpcss.css_color(BLACK);
	tmpclass.set_value(tmpcss);
	tmpclass.set_selector(css_class::link, tmpcss);
	tmpclass.set_selector(css_class::active, tmpcss);
	tmpclass.set_selector(css_class::visited, tmpcss);
	tmpcss.css_text_decoration(css::dc_underline);
	tmpclass.set_selector(css_class::hover, tmpcss);
	define_css_class_in_library(tmpclass);

	    // property for this component as a whole

	    // as it is nested in another components, it must
	    // have his own line, where from the clear property:
	tmpcss.clear();
	tmpcss.css_float_clear(css::fc_left);
	define_css_class_in_library(name_css_this, tmpcss);

	    // property for the shrink/expand/nosubdir button of the left directory tree

	tmpcss.clear();
	tmpcss.css_float_clear(css::fc_left);
	define_css_class_in_library(name_css_button, tmpcss);

	    // property for the subdirectory box (that will contain nested html_dir_tree objects

	tmpcss.clear();
	tmpcss.css_box_sizing(css::bx_border);
	tmpcss.css_float(css::fl_left);
	tmpcss.css_float_clear(css::fc_left);
	tmpcss.css_float_clear(css::fc_both);
	tmpcss.css_border_width(css::bd_left, css::bd_thin);
	tmpcss.css_border_style(css::bd_left, css::bd_solid);
	tmpcss.css_border_color(css::bd_left, COLOR_MENU_BORDER_OFF);
	tmpcss.css_margin_left("1em");
	define_css_class_in_library(name_css_forsubdirs, tmpcss);

	    // property for the table as a whole

	tmpcss.clear();
	tmpcss.css_box_sizing(css::bx_border);
	tmpcss.css_text_h_align(css::al_center);
	tmpcss.css_border_width(css::bd_all, css::bd_thin);
	tmpcss.css_border_style(css::bd_all, css::bd_dashed);
	tmpcss.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF);
	tmpcss.css_font_size("0.8em");
	define_css_class_in_library(name_css_contents, tmpcss);

	    // property for the title line

	tmpcss = webdar_css_style::get_css_class(webdar_css_style::btn_off).get_value();
	tmpcss.css_font_size("0.8em");
	define_css_class_in_library(name_css_contents_title, tmpcss);

	    // property for the table line contents (exculding the title line)

	tmpcss.clear();
	tmpcss.css_border_width(css::bd_top, css::bd_thin);
	tmpcss.css_border_style(css::bd_top, css::bd_dashed);
	tmpcss.css_border_color(css::bd_top, COLOR_MENU_BORDER_OFF);
	define_css_class_in_library(name_css_contents_cells, tmpcss);
    }
}

void html_dir_tree::clear_contents()
{
    contents.clear();
    contents.adopt_static_html(html_text(3, "File name").get_body_part());
    contents.adopt_static_html(html_text(3, "Delta Sig").get_body_part());
    contents.adopt_static_html(html_text(3, "Data").get_body_part());
    contents.adopt_static_html(html_text(3, "EA").get_body_part());
    contents.adopt_static_html(html_text(3, "compr").get_body_part());
    contents.adopt_static_html(html_text(3, "Sparse").get_body_part());
    contents.adopt_static_html(html_text(3, "permissions").get_body_part());
    contents.adopt_static_html(html_text(3, "UID").get_body_part());
    contents.adopt_static_html(html_text(3, "GID").get_body_part());
    contents.adopt_static_html(html_text(3, "Size").get_body_part());
    contents.adopt_static_html(html_text(3, "Modifaction Date").get_body_part());
}
