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

#ifndef HTML_SELECT_FILE_HPP
#define HTML_SELECT_FILE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <map>
#include <dar/libdar.hpp>
#include <memory>
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "events.hpp"
#include "actor.hpp"
#include "html_div.hpp"
#include "html_text.hpp"
#include "html_form_fieldset.hpp"
#include "html_button.hpp"
#include "html_double_button.hpp"
#include "html_table.hpp"
#include "html_popup.hpp"
#include "html_form.hpp"
#include "html_form_input.hpp"
#include "html_web_user_interaction.hpp"


    /// \file html_select_file.hpp defines html_select_file class


    /// class html_select_file

    /// given a libdar::entrepot object, the class html_select_file
    /// shows a popup for the user to navigate in the directory tree
    /// and select a file or a directory depending on the parameter set
    /// to this object
    ///
    /// usage for the caller is to:
    ///- adopt() this object
    ///- register for entry_selected and op_cancelled events
    /// on this object, events which will be triggered once the user will
    /// have selected a file or cancelled choice selection
    ///- call the different set_* methods
    /// when needed:
    ///- then call go_select() giving a libdar::entrepot (the object becomes visible)
    ///- and upon event entry_selected fetch the path selected by the user
    ///  calling get_selected_path()
    ///\note the object visibility is managed by go_select() and before triggering
    /// entry_select and op_cancelled events
    ///
    /** \verbatim
	+------------------------popup----------------------------+
	|+----------------title box------------------------------+|
	|| title                                                 ||
	|| entrepot_url                                          ||
	|| warning (hidden by default)                           ||
	|| webui (hidden by default)                             ||
	|+-------------------------------------------------------+|
	|+--------------- fieldset ------------------------------+|
	|| parentdir                                             ||
	|| +--------------+-----content------------------------+ ||
	|| |              |                                    | ||
	|| +--------------+------------------------------------+ ||
	|| |              |                                    | ||
	|| +--------------+------------------------------------+ ||
	|| |              |                                    | ||
	|| +--------------+------------------------------------+ ||
	|+-------------------------------------------------------+|
	|+-------------------btn_box-----------------------------+|
	||+--------+                          +--------++-------+||
	|||creatdir|                          |cancel  ||validate||
	||+--------+                          +--------++-------+||
	||+----- createdir_form------+                           ||
	||| createdir_input          |                           ||
	||+--------------------------+                           ||
	|+-------------------------------------------------------+|
	|                                                         |
	+---------------------------------------------------------+

	\endverbatim **/

class html_select_file: public html_popup, public events, public actor, public libthreadar::thread_signal
{
public:
	/// event when user selected a file
    static const std::string entry_selected;

	/// event when the user cancelled file selection
    static const std::string op_cancelled;

    enum select_mode
    {
	sel_file,    ///< only non dir and symlinks and non symlink should be selectable
	sel_dir,     ///< only directories and symlinks should be selectable
	sel_symlinks ///< only symlinks should be selectable
    };

	/// constructor

	/// \param[in] message is show to the user and should tell what it is expected to select for
    html_select_file(const std::string & message);

    html_select_file(const html_select_file & ref) = delete;
    html_select_file(html_select_file && ref) noexcept(false) = delete;
    html_select_file & operator = (const html_select_file & ref) = delete;
    html_select_file & operator = (html_select_file && ref) noexcept(false) = delete;
    virtual ~html_select_file() { clear_content(); cancel(); join(); };

	/// ask the user to select a file path (false) or a directory path (true)

	/// \note should be setup before calling go_select()
    void set_select_mode(select_mode val) { if(status != st_init) throw WEBDAR_BUG; cur_select_mode = val; };

	/// whether to show the button allowing the user to create a subdirectory

	/// \note should be setup before calling go_select()
    void set_can_create_dir(bool val) { if(status != st_init) throw WEBDAR_BUG; btn_createdir.set_visible(val); };

	/// only show directories and files matching the given glob expression (shell wildcards)
	/// \note giving an empty string disable filtering, all entries are shown
	/// which is the default behavior
    void set_filter(const std::string & mask) { filter = mask; };

	/// start the user interaction for a path selection

	/// \param[in] x_entr entrepot to scan directory from
	/// \param[in] start_dir absolute path in x_entr to take as starting directory
	/// \note if start_dir is not a directory not is an absolute path
	/// an exception is thrown
    void go_select(const std::shared_ptr<libdar::entrepot> & x_entr,
		   const std::string & start_dir);

	/// obtain the path selected by the user (mandatory after entry_selected event, in order to reuse this object)
    std::string get_selected_path() const { if(status != st_completed) throw WEBDAR_BUG; status = st_init; return fieldset.get_label(); };

	/// action triggered from html_buttons internally used in this class implementation (do not use, even if it must stay a public method)
    virtual void on_event(const std::string & event_name) override;

	/// returns the message argument passed at construction time
    std::string get_message() const { return x_message; };

protected:

	/// inherited from body_builder (via html_div)
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder (via html_div)
    virtual void new_css_library_available() override;

	/// inherited from libthreadar::thread
    virtual void inherited_run() override;

	/// inherited from libthreadar::thread
    virtual void signaled_inherited_cancel() override;

private:
	// event name for the 'change to parent directory' button (parentdir)
    static const std::string op_chdir_parent;
    static const std::string op_createdir;
    static const std::string op_hide_createdir;

	// internal css class names
    static const std::string css_float_button_right;
    static const std::string css_float_button_left;
    static const std::string css_warning;
    static const std::string css_sticky_top;
    static const std::string css_sticky_bot;

	// popup size of browser windows
    static constexpr const unsigned int width_pct = 80;
    static constexpr const unsigned int height_pct = 80;


	/// internal datastructure
    struct item
    {
	html_button* btn;
	libdar::inode_type type;
	item() { btn = nullptr; type = libdar::inode_type::unknown; };
	item(html_button* ptr) { btn = ptr; type = libdar::inode_type::unknown; };
    };


	// object status

    mutable enum
    {
	st_init,      ///< not showing popup for user selection and not path available
	st_go_select, ///< currently showing popup for user to select a file/directory
	st_completed  ///< not showing popup but path available using get_selected_path()
    } status;         ///< used to know when get_selected_path() is ready

    enum thread_to_run
    {
	run_nothing,   ///< nothing is expected to be run in inherited_run (error condition)
	run_create_dir,///< create_dir should be run
	run_init_fill, ///< init_fieldset_isdir() + fill_content() should be run sequentially
	run_fill_only  ///< fill_content() should be run
    };

    mutable thread_to_run which_thread; ///< used by inherited_run to know which subroutine to run in the separated thread


	// settings
    std::string x_message;            ///< message passed at constructor time

    select_mode cur_select_mode;      ///< whether user is expected to select a directory
    std::string filter;               ///< only display files matching this filter (glob expression)
    std::shared_ptr<libdar::entrepot> entr;  ///< the entrepot we should fetch info from
    std::shared_ptr<libdar::user_interaction> mem_ui;  ///< the original UI the entrepot had when given to go_select()
    bool should_refresh;              ///< expected status of the page refresh

	// html components

    html_text title;                  ///< shows the message request for the user (provided at constructor time)
    html_text entrepot_url;           ///< url of the entrepot
    html_text warning;                ///< shows warnings when needed (permission pb, etc.)
    html_web_user_interaction webui;  ///< used to control and interact with libdar thread used to act on the entrepot
    html_div title_box;               ///< contains title and warning and stay visible as a sticky box
    html_form_fieldset fieldset;      ///< shows the current directory path

	//
    std::string path_loaded;          ///< path displayed (empty string means not initialized)
    html_double_button parentdir;     ///< change to parent dir
    html_text content_placeholder;    ///< replace content and *parentdir* when loading the directory content
    html_table content;               ///< parent of content objects
    std::map<std::string, item> listed; ///< associate a event message to each listed items

    libthreadar::mutex content_mutex; ///< to avoid a subthread launched from on_event() to modify 'content' while also building body parts

    html_div btn_box;                 ///< box containing the bottom buttons
    html_button btn_cancel;           ///< triggers the entry_cancelled event
    html_button btn_validate;         ///< trigger the entry_selected event
    html_button btn_createdir;        ///< leads to create a new directory
    html_button btn_hide_createdir;   ///< hide the create dir form
    html_form createdir_form;         ///< form for new directory
    html_form_input createdir_input;  ///< field to let user provide new dir name


	// status field about html components
    bool is_loading_mode;             ///< whether the content placeholder shows
    bool fieldset_isdir;              ///< whether fieldset points to a directory or not


	// internal routines

	/// initialize fieldset_isdir field based on the content of fieldset, exception thrown if file/dir does not exist
	/// \return true if initialization could be performed
    bool init_fieldset_isdir();

	/// fills content and listed from entry located in fieldset.get_label() directory of the entr libdar::entrepot
    void fill_content();

	/// creates the requested directory
    void create_dir();

	/// used by fill_content() to add a single entry to the "listed" field
    void add_content_entry(const std::string & event_name,
			   libdar::inode_type pt,
			   const std::string & entry);

	/// run requested thread after sanity checks

	/// \note if run_nothing is provided an exception is thrown
    void run_thread(thread_to_run val);

	/// free up and clean all html_button of listed and clear content too in coherence
    void clear_content();

	/// set the display to show either the dir content or an "loading..." message

	/// \param[in] mode if set to true, change visibility to display the content_placeholder
	/// component, else show the normal components and available directory content
    void loading_mode(bool mode);

	/// return the parent path of the provided path
    static std::string get_parent_path(const std::string & somepath);

	/// join() wrapper

	/// \param[in] last whether we have to set back entrepot and forget about it
	/// \note when 'last' is false it only cleans up and restore states changed by run_thread()
	/// but if 'last' is true, this cleans up and restore states changed by go_select()
    void my_join(bool last);

};

#endif
