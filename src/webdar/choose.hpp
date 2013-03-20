#ifndef CHOOSE_HPP
#define CHOOSE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <vector>

    // webdar headers
#include "session.hpp"
#include "responder.hpp"
#include "html_page.hpp"
#include "html_page.hpp"
#include "html_url.hpp"
#include "html_form.hpp"
#include "html_div.hpp"
#include "html_table.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_input.hpp"
#include "html_form_radio.hpp"

class choose : public responder
{
public:

    static answer give_answer_for(const std::string & user, const request & req);

    static void cleanup_memory();

private:

	// constructor
    choose(const std::string & user);
    ~choose() { release_boxes(); };

	// inherited from responder
    answer give_answer(const request & req);


    std::string owner;       //< list only sessions for that owner
    bool confirm_mode;       //< whether we display session list or kill confirmation
	/// listing session page and associated objects
    html_page page;         //< page root for session listing
    html_table table;
    html_url nouvelle;
    html_form form;
    html_div div;
    std::vector<html_form_input *> boxes; //< list of checkboxes in "page"
    std::vector<session::session_summary> sess; //< list of sessions in page

	/// confirmation page and associated objects
    html_page confirm;      //< page root for kill confirmation
    html_div cdiv;
    html_table ctable;
    html_form cform;
    html_form_fieldset block;
    html_form_radio radio;


    answer create_new_session(const request & req);
    void regenerate_table_page();
    void release_boxes();
    void regenerate_confirm_page();
    void kill_selected_sessions() const;

	// class structure

    struct record
    {
	choose *obj;
	mutex lock;
	record() { obj = NULL; };
	~record() { if(obj != NULL) delete obj; };
    };

    static std::map<std::string, record *> per_user;
};

#endif
