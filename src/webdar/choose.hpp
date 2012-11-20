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
    ~choose();

	// inherited from responder
    answer give_answer(const request & req);


    std::string owner;       //< list only sessions for that owner
    bool confirm_mode;       //< whether we display session list or kill confirmation
    html_page *page;         //< page root for session listing
    std::vector<html_form_input *> boxes; //< list of checkboxes in "page"
    html_page *confirm;      //< page root for kill confirmation
    html_form_radio *confirmed; //< radio button for user confirmation
    std::vector<session::session_summary> sess; //< list of sessions in page

    answer create_new_session(const request & req);
    void regenerate_table_page();
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
