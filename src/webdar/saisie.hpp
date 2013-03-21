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
    aiguille show_archive; ///< top middle zone hidden or show depending of choice's value
    html_url sessions;     ///< below the menu, points to the all sessions page
    aiguille select;       ///< middle center zone, containing the different pages according to choice's value

	/// show_archive zone
    html_form_input archive; ///< child of show_archive, contains the archive name
    html_text archive_blank;
    html_form archive_form;

	// the different sub pages contained by "select" and shown depending on choice's value
    html_div about;
    html_div compare;
    html_div test;
    options_listing list;
    html_div create;
    html_div isolate;
    html_div merge;
    html_div filters;
    html_div sep;
    html_yes_no_box close;

	// the restore sub-page
    html_form ex_form;
    html_form_fieldset ex_fs;
    options_extract extract;

};




#endif
