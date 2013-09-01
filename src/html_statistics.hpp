/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
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

#ifndef HTML_STATISTICS_HPP
#define HTML_STATISTICS_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <dar/libdar.hpp>


    // webdar headers
#include "body_builder.hpp"
#include "html_table.hpp"
#include "html_text.hpp"

class html_statistics : public body_builder
{
public:
    html_statistics();
    html_statistics(const html_statistics & ref) { throw WEBDAR_BUG; };
    const html_statistics operator = (const html_statistics & ref) { throw WEBDAR_BUG; };
    ~html_statistics() { unbuild(); };

    void clear_counters() { stats.clear(); };
    void clear_labels();

	// fields that do not get a label are not shown
    void set_treated_label(const std::string & label) { treated_lbl.clear(); treated_lbl.add_text(0, label); unbuild(); };
    void set_hard_links_label(const std::string & label) { hard_links_lbl.clear(); hard_links_lbl.add_text(0, label); unbuild(); };
    void set_skipped_label(const std::string & label) { skipped_lbl.clear(); skipped_lbl.add_text(0, label); unbuild(); };
    void set_ignored_label(const std::string & label) { ignored_lbl.clear(); ignored_lbl.add_text(0, label); unbuild(); };
    void set_tooold_label(const std::string & label) { tooold_lbl.clear(); tooold_lbl.add_text(0, label); unbuild(); };
    void set_errored_label(const std::string & label) { errored_lbl.clear(); errored_lbl.add_text(0, label); unbuild(); };
    void set_deleted_label(const std::string & label) { deleted_lbl.clear(); deleted_lbl.add_text(0, label); unbuild(); };
    void set_ea_treated_label(const std::string & label) { ea_treated_lbl.clear(); ea_treated_lbl.add_text(0, label); unbuild(); };
    void set_byte_amount_label(const std::string & label) { byte_amount_lbl.clear(); byte_amount_lbl.add_text(0, label); unbuild(); };
    void set_total_label(const std::string & label) { total_lbl.clear(); total_lbl.add_text(0, label); unbuild(); };

	/// the address of the object to be updated by libdar
    libdar::statistics *get_libdar_statistics() { return & stats; };

	/// inherited from body_builder class
	///
	/// \note always fields that got a label (using a set_*_label() method)
	/// are displayed.
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    libdar::statistics stats;

    html_table *table;

    html_text treated_lbl;
    html_text hard_links_lbl;
    html_text skipped_lbl;
    html_text ignored_lbl;
    html_text tooold_lbl;
    html_text errored_lbl;
    html_text deleted_lbl;
    html_text ea_treated_lbl;
    html_text byte_amount_lbl;
    html_text total_lbl;

    html_text treated_count;
    html_text hard_links_count;
    html_text skipped_count;
    html_text ignored_count;
    html_text tooold_count;
    html_text errored_count;
    html_text deleted_count;
    html_text ea_treated_count;
    html_text byte_amount_count;
    html_text total_count;

	/// build the html body_builder tree using only labelled field
    void build();

	/// destroy the table to force a build() at the next cycle
    void unbuild();
};

#endif
