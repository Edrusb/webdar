#ifndef HTML_LISTING_PAGE_HPP
#define HTML_LISTING_PAGE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_page.hpp"
#include "html_dir_tree.hpp"
#include "html_button.hpp"
#include "html_div.hpp"
#include "archive_init_list.hpp"
#include "html_url_class.hpp"

class html_listing_page : public html_page, public actor, public events
{
public:
    static const std::string event_close;

    html_listing_page();

	/// set title of the listing page given the session name
    void set_session_name(const std::string & session_name);

	/// mandatory call before calling get_body_part !!!
    void set_source(const archive_init_list *ref) { tree.set_source(ref); };

	/// clear informations about previously read archive
    void clear() { tree.clear(); };

	// inherited from actor
    virtual void on_event(const std::string & event_name);

protected:
	/// inherited from html_div/body_builder
    virtual void path_has_changed() { html_page::path_has_changed(); tree.set_css_classid(bt_class.get_class_id()); };


private:
    html_dir_tree tree;
    html_button close;
    html_url_class bt_class;
};


#endif
