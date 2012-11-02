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

	/// returns whether the request is authoritative
	///
	/// \param[in] req the request to analyse
	/// \param[out] user the authenticated user
	/// \return true when the request is authoritative and the set "user" to the name of the authenticated user
    bool is_an_authoritative_request(const request & req, std::string & user);

	/// to be used to answer a non authoritative request
    answer give_answer(const request & req);

private:
    const authentication *database;

};

#endif
