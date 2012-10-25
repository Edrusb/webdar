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

    answer give_answer(const request & req)
    {
	answer ret;
	ret.set_status(status);
	ret.set_reason(msg);
	ret.add_body(std::string("ERROR WHILE REQUESTING THIS URL: ") + req.get_uri().get_string());
	return ret;
    };

private:
    unsigned int status;
    std::string msg;

};


#endif
