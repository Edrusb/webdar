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
    menu *choice;
    aiguille *select;
    aiguille *show_archive;
    html_url *sessions;
    html_form_input *archive;

	// the different sub pages:
    html_div *about;
    html_div *restore;
    html_div *compare;
    html_div *test;
    options_listing *list;
    html_div *create;
    html_div *isolate;
    html_div *merge;
    html_div *filters;
    html_div *sep;
    html_yes_no_box *close;

    void set_show_archive();
    void set_about();
    void set_restore();
    void set_compare();
    void set_test();
    void set_list();
    void set_create();
    void set_isolate();
    void set_merge();
    void set_filters();
    void set_close();
};




#endif
