/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2022 Denis Corbin
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
#include "tokens.hpp"
#include "html_table.hpp"
#include "html_text.hpp"

    //
#include "html_dir_tree.hpp"

using namespace std;

const string html_dir_tree::event_shrink = "html_dir_tree_shrink";
const string html_dir_tree::event_expand = "html_dir_tree_expand";
const string html_dir_tree::event_click = "html_dir_tree_click";

html_dir_tree::html_dir_tree(const std::string & chemin):
    shrink("-", event_shrink),
    expand("+", event_expand),
    name("", event_click),
    nosubdir(" ",""),
    contents(10)
{
    init(chemin);
}

html_dir_tree::html_dir_tree(const archive_init_list * ref, const std::string & chemin):
    shrink("-", event_shrink),
    expand("+", event_expand),
    name("", event_click),
    nosubdir(" ",""),
    contents(10)
{
    init(chemin);
    set_source(ref);
}

void html_dir_tree::init(const std::string & chemin)
{
    css tmpcss;

    my_path = chemin;
    src = nullptr;
    info_read = false;
    has_sub = true; // until checked it has or has not
    last_classid = "";
    visibility_has_changed = false;
    focus_place = nullptr;
    focus_title = nullptr;

    if(chemin == "")
	name.change_label("&lt;ROOT&gt;");
    else
	name.change_label(my_path.back());

	// setting css properties
    set_no_CR();

    line.css_float(css::fl_left, true);
    line.css_float_clear(css::fc_both, true);
    line.css_width("100%", false);

    shrink.css_float(css::fl_left, true);
    shrink.css_float_clear(css::fc_both, true);
    shrink.css_width("2em", false, true);
    shrink.css_height("1em", false, true);
    shrink.css_text_align(css::al_center, true);
    shrink.css_margin_right("1em", true);
    shrink.css_border_width(css::bd_all, css::bd_medium, true);
    shrink.css_border_style(css::bd_all, css::bd_solid, true);

    nosubdir.css_inherit_from(shrink); // propagate the just defined css value from shrink to nosubdir
    nosubdir.css_border_color(css::bd_all, COLOR_BACK);
    nosubdir.css_height("1em", false);

    shrink.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF, true);

    expand.css_inherit_from(shrink); // propagate all defined css value from shrink to expand

    for_subdirs.css_float(css::fl_left);
    for_subdirs.css_float_clear(css::fc_both);
    for_subdirs.css_width("100%", false);
    for_subdirs.css_border_width(css::bd_left, css::bd_thin);
    for_subdirs.css_border_style(css::bd_left, css::bd_solid);
    for_subdirs.css_border_color(css::bd_left, COLOR_MENU_BORDER_OFF);
    for_subdirs.css_margin_left("1em");

	// to have the name of the directory beside the shrink/expand button
    line.set_no_CR();
    nosubdir.set_no_CR();
    shrink.set_no_CR();
    expand.set_no_CR();
    name.set_no_CR();

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

	// set initial visibility of objects
    on_event(event_shrink);

	// contents is not directly adopted and is filled with go_init_indent()
    contents.adopt_static_html(html_text(3, "Filename").get_body_part());
    contents.adopt_static_html(html_text(3, "Data").get_body_part());
    contents.adopt_static_html(html_text(3, "EA").get_body_part());
    contents.adopt_static_html(html_text(3, "compr").get_body_part());
    contents.adopt_static_html(html_text(3, "Sparse").get_body_part());
    contents.adopt_static_html(html_text(3, "permissions").get_body_part());
    contents.adopt_static_html(html_text(3, "UID").get_body_part());
    contents.adopt_static_html(html_text(3, "GID").get_body_part());
    contents.adopt_static_html(html_text(3, "Size").get_body_part());
    contents.adopt_static_html(html_text(3, "Modifaction Date").get_body_part());
    contents.css_text_align(css::al_center);
    contents.css_border_width(css::bd_all, css::bd_thin);
    contents.css_border_style(css::bd_all, css::bd_dashed);
    contents.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF);
	// assigning style for all lines
    tmpcss.css_clear_attributes();
    tmpcss.css_border_width(css::bd_top, css::bd_thin);
    tmpcss.css_border_style(css::bd_top, css::bd_dashed);
    tmpcss.css_border_color(css::bd_top, COLOR_MENU_BORDER_OFF);
    contents.set_css_cells(tmpcss);
	// assigning modified style for header line
    tmpcss.css_background_color(COLOR_MENU_BACK_OFF);
    tmpcss.css_color(COLOR_MENU_FRONT_OFF);
    contents.set_css_cells_first_raw(tmpcss);
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
    }
}

void html_dir_tree::set_css_classid(const std::string & classid)
{
    last_classid = classid; // to be able to set future subdirectories
    shrink.set_url_classid(classid);
    expand.set_url_classid(classid);
    name.set_url_classid(classid);
    for(vector<html_dir_tree *>::iterator it = subdirs.begin();
	it != subdirs.end();
	++it)
	if(*it == nullptr)
	    throw WEBDAR_BUG;
	else
	    (*it)->set_css_classid(classid);
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

void html_dir_tree::on_event(const std::string & event_name)
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
	    focus_title->clear();
	    focus_title->adopt_static_html(my_path.display(false));
	}
	go_init_indent();
    }
    else
	throw WEBDAR_BUG;
}

string html_dir_tree::get_body_part(const chemin & path,
				   const request & req)
{
    string ret;

    do
    {
	visibility_has_changed = false;
	ret = html_div::get_body_part(path, req);
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

	while(it != tmp.end())
	{
	    if(it->is_dir())
	    {
		html_dir_tree *tmp_sub = new (nothrow) html_dir_tree(src, (my_path + chemin(it->get_name())).display(true));

		if(tmp_sub == nullptr)
		    throw exception_memory();

		try
		{
		    if(last_classid != "")
			tmp_sub->set_css_classid(last_classid);
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
