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

#ifndef HTML_DIR_TREE_HPP
#define HTML_DIR_TREE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <vector>

    // webdar headers
#include "html_div.hpp"
#include "events.hpp"
#include "chemin.hpp"
#include "archive_init_list.hpp"
#include "html_button.hpp"
#include "html_focus.hpp"
#include "html_table.hpp"


class html_dir_tree: public html_div, public actor
{
public:
    html_dir_tree(const std::string & chemin); // minimal constructeur, must use set_source() ASAP
    html_dir_tree(const archive_init_list * ref, const std::string & chemin);
    html_dir_tree(const html_dir_tree & ref):shrink("",""),expand("",""),name("",""), nosubdir("",""), contents(1) { throw WEBDAR_BUG; };
    const html_dir_tree & operator = (const html_dir_tree & ref) { throw WEBDAR_BUG; };
    ~html_dir_tree() { clear(); };

	/// clear information learnt from a previously opened archive
    void clear();

	/// where to fetch from archive content
    void set_source(const archive_init_list *ref);

	/// where to attach to the archive contents when "this" is selected
    void set_drop_content(html_focus *ref) { focus_place = ref; };

	/// where to write the path when under focus
    void set_drop_path(html_div *ref) { focus_title = ref; };

	/// which URL class to use for embedded buttons
    void set_css_classid(const std::string & classid);

	/// shrink the directory
    void go_shrink();

	/// expand the directory exposing all its subdirectories
    void go_expand();

	/// show contents in the focus place
    void go_show() { on_event(event_click); };

	/// inherited from actor
    virtual void on_event(const std::string & event_name);

	/// inherite from html_div
    virtual std::string get_body_part(const chemin & path,
				      const request & req);


private:
    static const std::string event_shrink;
    static const std::string event_expand;
    static const std::string event_click;

    chemin my_path;
    const archive_init_list *src;
    bool info_read;
    bool has_sub;
    std::string last_classid;
    bool visibility_has_changed; //< whether we need to rebuild the body_builder answer
    std::vector<html_dir_tree *> subdirs; //< subdirectories of the tree

	// HTML controls
    html_div line;      //< contains shrink/expand/nosubdir buttons + name for alignment
    html_button shrink; //< alternatively visible button
    html_button expand; //< alternatively visible button
    html_button nosubdir; //< "shown" when no subdir exist
    html_button name;   //< name of the directory
    html_div for_subdirs; //< where take place subdirs
    html_focus *focus_place; //< where to send contents upon focus
    html_table contents; //< directory contents
    html_div *focus_title; //< where to write title of the contents when under focus

    void init(const std::string & chemin);
    void go_init_indent();
    void go_hide();
};

#endif
