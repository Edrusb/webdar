#ifndef CHALLENGE_HPP
#define CHALLENGE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "responder.hpp"
#include "authentication.hpp"


class challenge: public responder
{
public:
	/// constructor
	///
	/// \param[in] base points to the authentication base
	/// \note the base argument must survive this challenge object and is
    challenge(const authentication *base) { if(base == NULL) throw WEBDAR_BUG; database = base; };

	/// inherited from responder
    answer give_answer(const request & req);

private:
    const authentication *database;

    void set_answer_need_authorization(answer & ans);
    void set_answer_redirect_to_choose(answer & ans, const std::string & user);
    void set_answer_from_chooser_object(answer & ans, const std::string & user,
					const request & req);
};

#endif
