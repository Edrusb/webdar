#ifndef CHOOSE_HPP
#define CHOOSE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "session.hpp"
#include "responder.hpp"

class choose : public responder
{
public:
	// constructor
    choose(const std::string & user) { owner = user; };

	// inherited from responder
    answer give_answer(const request & req);

private:
    std::string owner;
    answer create_new_session();
};

#endif
