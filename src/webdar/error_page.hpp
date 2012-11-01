#ifndef ERROR_PAGE_HPP
#define ERROR_PAGE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "responder.hpp"

class error_page : public responder
{
public:
    error_page(unsigned int status_code,
	       const std::string & reason) { status = status_code; msg = reason; };

    void set_message_body(const std::string & message) { body = message; };

    answer give_answer(const request & req);

private:
    unsigned int status;
    std::string msg;
    std::string body;

};


#endif
