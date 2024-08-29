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
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_form_overwrite_base_criterium.hpp"

using namespace std;


html_form_overwrite_base_criterium::html_form_overwrite_base_criterium():
    crit_fs(""),
    negate("Not", html_form_input::check, "", 1),
    invert("Invert roles", html_form_input::check, "", 1),
    base("Criterium"),
    date("Date"),
    hourshift("Hourshift", html_form_input::number, "0", 5)
{

	// components setup
    base.add_choice("crit_in_place_is_inode", "in place entry is an inode");
    base.add_choice("crit_in_place_is_dir", "in place entry is a directory");
    base.add_choice("crit_in_place_is_file", "in place entry is a plain file");
    base.add_choice("crit_in_place_is_hardlinked_inode", "in place entry is a hard linked inode");
    base.add_choice("crit_in_place_is_new_hardlinked_inode", "in place entry is a hard linked inode we meet for the first time");
    base.add_choice("crit_in_place_data_more_recent", "in place inode has more recent data");
    base.add_choice("crit_in_place_data_more_recent_or_equal_to", "in place inode a data more recent than or equal to: ");
    base.add_choice("crit_in_place_data_bigger", "both are plain files and in place one is bigger file");
    base.add_choice("crit_in_place_data_saved", "in place has its data saved or is not an inode");
    base.add_choice("crit_in_place_data_dirty", "in place entry is a dirty file");
    base.add_choice("crit_in_place_data_sparse", "in place entry is a sparse file");
    base.add_choice("crit_in_place_has_delta_sig", "in place entry has a delta signature attached");
    base.add_choice("crit_same_inode_data", "both entry are of the same type and have the same metadata");
    base.add_choice("crit_in_place_EA_present", "in place entry has Extended Attributes");
    base.add_choice("crit_in_place_EA_more_recent", "in place entry has more recent Extended Attributes");
    base.add_choice("crit_in_place_EA_more_recent_or_equal_to", "in place entry has EA more recent than or equal to:");
    base.add_choice("crit_in_place_more_EA", "in place entry has more Extended Attributes");
    base.add_choice("crit_in_place_EA_bigger", "in place entry has bigger Extended Attributes");
    base.add_choice("crit_in_place_EA_saved", "in place entry has Extended Attributes saved");
    base.add_choice("crit_same_type", "both entries are of the same type");

    hourshift.set_range(0, 24);

	// adoption tree
    crit_fs.adopt(&negate);
    crit_fs.adopt(&invert);
    crit_fs.adopt(&base);
    crit_fs.adopt(&date);
    crit_fs.adopt(&hourshift);
    adopt(&crit_fs);

	// events
    base.record_actor_on_event(this, html_form_select::changed);

    	// set default visibility
    on_event(html_form_select::changed);

	// css

}

unique_ptr<libdar::criterium> html_form_overwrite_base_criterium::get_overwriting_criterium() const
{
    unique_ptr<libdar::criterium> ret;

    if(base.get_selected_id() == "crit_in_place_is_inode")
	ret.reset(new (nothrow) libdar::crit_in_place_is_inode());
    else if(base.get_selected_id() == "crit_in_place_is_dir")
	ret.reset(new (nothrow) libdar::crit_in_place_is_dir());
    else if(base.get_selected_id() == "crit_in_place_is_file")
	ret.reset(new (nothrow) libdar::crit_in_place_is_file());
    else if(base.get_selected_id() == "crit_in_place_is_hardlinked_inode")
	ret.reset(new (nothrow) libdar::crit_in_place_is_hardlinked_inode());
    else if(base.get_selected_id() == "crit_in_place_is_new_hardlinked_inode")
	ret.reset(new (nothrow) libdar::crit_in_place_is_new_hardlinked_inode());
    else if(base.get_selected_id() == "crit_in_place_data_more_recent")
	ret.reset(new (nothrow) libdar::crit_in_place_data_more_recent(libdar::deci(hourshift.get_value()).computer()));
    else if(base.get_selected_id() == "crit_in_place_data_more_recent_or_equal_to")
	ret.reset(new (nothrow) libdar::crit_in_place_data_more_recent_or_equal_to(date.get_value(),
										   libdar::deci(hourshift.get_value()).computer()));
    else if(base.get_selected_id() == "crit_in_place_data_bigger")
	ret.reset(new (nothrow) libdar::crit_in_place_data_bigger());
    else if(base.get_selected_id() == "crit_in_place_data_saved")
	ret.reset(new (nothrow) libdar::crit_in_place_data_saved());
    else if(base.get_selected_id() == "crit_in_place_data_dirty")
	ret.reset(new (nothrow) libdar::crit_in_place_data_dirty());
    else if(base.get_selected_id() == "crit_in_place_data_sparse")
	ret.reset(new (nothrow) libdar::crit_in_place_data_sparse());
    else if(base.get_selected_id() == "crit_in_place_has_delta_sig")
	ret.reset(new (nothrow) libdar::crit_in_place_has_delta_sig());
    else if(base.get_selected_id() == "crit_same_inode_data")
	ret.reset(new (nothrow) libdar::crit_same_inode_data());
    else if(base.get_selected_id() == "crit_in_place_EA_present")
	ret.reset(new (nothrow) libdar::crit_in_place_EA_present());
    else if(base.get_selected_id() == "crit_in_place_EA_more_recent")
	ret.reset(new (nothrow) libdar::crit_in_place_EA_more_recent(libdar::deci(hourshift.get_value()).computer()));
    else if(base.get_selected_id() == "crit_in_place_EA_more_recent_or_equal_to")
	ret.reset(new (nothrow) libdar::crit_in_place_EA_more_recent_or_equal_to(date.get_value(),
										 libdar::deci(hourshift.get_value()).computer()));
    else if(base.get_selected_id() == "crit_in_place_more_EA")
	ret.reset(new (nothrow) libdar::crit_in_place_more_EA());
    else if(base.get_selected_id() == "crit_in_place_EA_bigger")
	ret.reset(new (nothrow) libdar::crit_in_place_EA_bigger());
    else if(base.get_selected_id() == "crit_in_place_EA_saved")
	ret.reset(new (nothrow) libdar::crit_in_place_EA_saved());
    else if(base.get_selected_id() == "crit_same_type")
	ret.reset(new (nothrow) libdar::crit_same_type());
    else
	throw WEBDAR_BUG;

    if(!ret)
	throw exception_memory();

    if(invert.get_value_as_bool()) // "Invert role" checkbox is checked
    {
	ret.reset(new (nothrow) libdar::crit_invert(*ret));
	if(!ret)
	    throw exception_memory();
    }

    if(negate.get_value_as_bool()) // "Not" checkbox is checked
    {
	ret.reset(new (nothrow) libdar::crit_not(*ret));
	if(!ret)
	    throw exception_memory();
    }

    return ret;
}


void html_form_overwrite_base_criterium::on_event(const std::string & event_name)
{
    if(event_name == html_form_select::changed)
    {
	if(base.get_selected_id() == "crit_in_place_data_more_recent_or_equal_to" ||
	   base.get_selected_id() == "crit_in_place_EA_more_recent_or_equal_to")
	{
	    date.set_visible(true);
	    hourshift.set_visible(true);
	}
	else
	{
	    date.set_visible(false);
	    hourshift.set_visible(false);
	}
    }
    else
	throw WEBDAR_BUG;
}


string html_form_overwrite_base_criterium::inherited_get_body_part(const chemin & path,
								   const request & req)
{
    return get_body_part_from_all_children(path, req);
}

