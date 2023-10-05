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

#ifndef SAISIE_HPP
#define SAISIE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_page.hpp"
#include "html_menu.hpp"
#include "html_aiguille.hpp"
#include "html_div.hpp"
#include "events.hpp"
#include "html_yes_no_box.hpp"
#include "html_options_extract.hpp"
#include "html_options_read.hpp"
#include "html_options_compare.hpp"
#include "html_options_test.hpp"
#include "html_archive_read.hpp"
#include "html_archive_create.hpp"
#include "html_archive_isolate.hpp"
#include "html_archive_merge.hpp"
#include "html_button.hpp"


    /// class saisie
    ///
    /// defines the web "pages" when no operation is
    /// running in the current session.
    ///- on the left a menu 'choices' that triggers 'this' to modify the middle and top area
    ///- on the top midle an html_aiguille 'show_archive' which
    /// contains two alternatives, nothing or the archive path
    ///- just below and beside the menu, an html_aiguille "select" that
    /// shows the different fields in the context of the menu "choices"

class saisie : public html_page, public actor, public events
{
public:
	/// available event for that class
    static const std::string event_closing;
    static const std::string event_restore;
    static const std::string event_compare;
    static const std::string event_test;
    static const std::string event_list;
    static const std::string event_create;
    static const std::string event_isolate;
    static const std::string event_merge;
    static const std::string changed_session_name;

	/// constructor
    saisie();
    saisie(const saisie & ref) = default;
    saisie(saisie && ref) noexcept = default;
    saisie & operator = (const saisie & ref) = default;
    saisie & operator = (saisie && ref) noexcept = default;
    ~saisie() = default;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;


	// list of field available to run libdar

	// reading parameters
    const std::string & get_archive_path() const;
    const std::string & get_archive_basename() const;
    const libdar::archive_options_read & get_read_options() const;

	// common parameter to diff/create/restore
    const std::string & get_fs_root() const;


	// extraction parameters
    const libdar::archive_options_extract get_extraction_options() const;

	// comparison parameters
    const libdar::archive_options_diff get_comparison_options() const;

	// testing options
    const libdar::archive_options_test get_testing_options() const;

	// create options --- WARNING! The returned object is not a libdar object but holds the information to create it completely
    const html_options_create & get_creating_options() const;

	// isolate options
    const libdar::archive_options_isolate get_isolating_options() const;
    const html_archive_read & get_isolating_reference() const { return isolate.get_reference(); };

	// merge options --- WARNING! here too the returned object is not a libda object
    const html_options_merge & get_merging_options() const;
    const html_archive_read & get_merging_reference() const { return merge.get_reference(); };

	/// defines the name of the session
	///
	/// \note this call does not trigger any event
    void set_session_name(const std::string & name) { session_name.set_value(name); };

	/// get the current session name
    std::string get_session_name() const { return session_name.get_value(); };

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    enum
    {
	st_idle,    //< user did not click any action button, or action has completed
	st_restore, //< user clicked on the restore button
	st_compare, //< user clicked on the compare button
	st_test,    //< user clicked on the test button
	st_list,    //< user clicked on the list button
	st_create,  //< user clicked on the create button
	st_isolate, //< user clicked on the isolate button
	st_merge    //< user clicked on the merge button
    } status;       //< status is used to determine which field to user to provide information toward libdar

    html_menu choice;      ///< left main menu
    html_div right_pan;    ///< holds all that is beside main menu

	/// show archive zone (middle top)
    html_div archive_show;   ///< global container that is either visible or not depending on "choice" value
    html_archive_read archread;   ///< the archive to operate on (reading)
    html_form_input show_operation_options; ///< whether operation options are shown or not
    html_form show_archive_form_options;    ///< around show_archive_fs_options
    html_form_fieldset show_archive_fs_options; //< around show_operations_options

	/// right_pan zone
    html_aiguille select;         ///< middle center zone, containing the different pages according to "choice" value

	// the different sub pages contained by "select" and shown depending on choice's value
    html_div div_about;
    html_div around_licensing;
    html_url licensing;
    html_form_input session_name;
    html_form_fieldset about_fs;
    html_form about_form;
	//
    html_div div_sep0;
	//
    html_div div_extract;
    html_form_fieldset extract_fs_root_fs;
    html_form_input extract_fs_root;
    html_form extract_fs_root_form;
    html_options_extract extract;
    html_button go_extract;
	//
    html_div div_compare;
    html_form_fieldset diff_fs_root_fs;
    html_form_input diff_fs_root;
    html_form diff_fs_root_form;
    html_options_compare compare;
    html_button go_compare;
	//
    html_div div_test;
    html_options_test test;
    html_button go_test;
	//
    html_div div_list;
    html_button go_list;
	//
    html_div div_create;
    html_archive_create create;
    html_button go_create;
	//
    html_div div_isolate;
    html_archive_isolate isolate;
    html_button go_isolate;
	//
    html_div div_merge;
    html_archive_merge merge;
    html_button go_merge;
	//
    html_div div_sep1;
	//
    html_div div_biblio;
	//
    html_div div_sess;
	//
    html_div div_sep2;
	//
    html_yes_no_box close;

    static const std::string css_class_text;

    static const std::string menu_main;
    static const std::string menu_restore;
    static const std::string menu_compare;
    static const std::string menu_test;
    static const std::string menu_list;
    static const std::string menu_create;
    static const std::string menu_isolate;
    static const std::string menu_merge;
    static const std::string menu_biblio;
    static const std::string menu_sessions;
    static const std::string menu_close;


};

#endif
