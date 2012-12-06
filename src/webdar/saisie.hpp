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


class saisie : public html_page
{
public:
    saisie();

	/// inherited from html_page
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    menu *choice;
    aiguille *select;
    html_url *sessions;
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
    html_div *close;
};




#endif
