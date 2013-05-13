#ifndef SAISIE_HPP
#define SAISIE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_page.hpp"
#include "menu.hpp"
#include "aiguille.hpp"
#include "html_div.hpp"
#include "options_listing.hpp"
#include "events.hpp"
#include "html_yes_no_box.hpp"
#include "options_extract.hpp"
#include "options_read.hpp"
#include "options_compare.hpp"
#include "options_test.hpp"
#include "archive_read.hpp"
#include "archive_create.hpp"
#include "html_button.hpp"
#include "html_url_class.hpp"

    /// class saisie
    ///
    /// defines the web "pages" when no operation is
    /// running in the current session.
    /// .on the left a menu 'choices' that triggers 'this' to modify the middle and top areasfu:
    /// .on the top midle an aiguille 'show_archive' which
    /// contains two alternatives, nothing or the archive path
    /// .just below and beside the menu, an aiguille "select" that
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

	/// constructor
    saisie();

	/// inherited from html_page
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// inherited from actor
    void on_event(const std::string & event_name);
private:
    menu choice;           ///< left main menu
    html_div right_pan;    ///< holds all that is beside main menu

	/// right_pan zone
    aiguille show_archive;   ///< top middle zone hidden or show depending of choice's value
    aiguille select;         ///< middle center zone, containing the different pages according to choice's value

	/// show archive zone
    html_div archive_show;   ///< contains all that is "shown"
    html_text archive_blank; ///< shows a blank place in place of archive_form
	/// what is shown
    archive_read archread;   ///< the archive to operate on (reading)
    html_form_input show_operation_options; ///< whether operation options are shown or not
    html_form_fieldset show_archive_fs_options; //< around  show_operations_options
    html_form show_archive_form_options;    ///< around show_archive_fs_options


	// the different sub pages contained by "select" and shown depending on choice's value
    html_div div_about;
	//
    html_div div_sep0;
	//
    html_div div_extract;
    options_extract extract;
    html_button go_extract;
	//
    html_div div_compare;
    options_compare compare;
    html_button go_compare;
	//
    html_div div_test;
    options_test test;
    html_button go_test;
	//
    html_div div_list;
    options_listing list;
    html_button go_list;
	//
    html_div div_create;
    archive_create create;
    html_button go_create;
	//
    html_div div_isolate;
    html_button go_isolate;
	//
    html_div div_merge;
    html_button go_merge;
	//
    html_div div_sep1;
	//
    html_div div_filters;
	//
    html_div div_repo;
	//
    html_div div_sess;
	//
    html_div div_sep2;
	//
    html_yes_no_box close;

	// CSS attributes to buttons and menu
    html_div box_off; //< used to assign CSS attributes: unselected item
    html_div box_on;  //< used to assign CSS attributes: selected item
    html_div box_void;//< used to assign CSS attributes: separators
    html_url_class url_selected; //< links when box selected
    html_url_class url_normal;   //< links when box no selected
};

#endif
