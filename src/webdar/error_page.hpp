#ifndef ERROR_PAGE_HPP
#define ERROR_PAGE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "responder.hpp"
#include "html_page.hpp"
#include "html_text.hpp"
#include "html_url.hpp"
#include "html_form_input.hpp"
#include "html_form.hpp"
#include "html_div.hpp"

class error_page : public responder
{
public:
    error_page(unsigned int status_code,
	       const std::string & reason);

    void set_message_body(const std::string & message);
    void set_return_uri(const uri &ref, const std::string & label);

	/// inherited from responder
    answer give_answer(const request & req);

protected:
    virtual void prefix_has_changed();

private:
    unsigned int status;  //< return code of the answer (seen by browser, rarely visibile by the user)
    std::string msg;      //< returned reason of the answer ( seen by browser, rarely visibile by the user)

	// body_builder objects
    html_page page;      //< html construction primitive
    html_div div;        //< construction object containing both body and text
    html_text body;      //< message displayed to the user set by 'set_message_body()'
    html_text text;      //< used to build the return path in HTML body

    void field_delete();
};


#endif
