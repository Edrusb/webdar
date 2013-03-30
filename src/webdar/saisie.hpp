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
    static const std::string closing;


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
    html_form archive_form;  ///< form containing the path to the archive to operate on
    html_form_input archive; ///< child of archive_form, contains the archive name
    html_form_input show_read_options;
    html_form_input show_operation_options;
    options_read opt_read;   ///< read options

	// the different sub pages contained by "select" and shown depending on choice's value
    html_div about;
    html_div sep0;
    options_extract extract;
    html_div compare;
    html_div test;
    options_listing list;
    html_div create;
    html_div isolate;
    html_div merge;
    html_div sep1;
    html_div filters;
    html_div repo;
    html_div sess;
    html_div sep2;
    html_yes_no_box close;

};




#endif
