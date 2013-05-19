#ifndef RESPONDER_HPP
#define RESPONDER_HPP

    // C++ system header files
#include <string>
#include <vector>
#include <map>

    // webdar headers
#include "uri.hpp"
#include "webdar_tools.hpp"
#include "exceptions.hpp"
#include "request.hpp"
#include "answer.hpp"
#include "chemin.hpp"

    /////////////////////////////////////////////////////////////////////////////////////
    /// pure virtual class, base class for all classes that can generate an anwser to a request

class responder
{
public:
    virtual ~responder() {};

	/// used for body_builder object for they have a proper prefix in path
    void set_prefix(const chemin & chem) { prefix = chem; prefix_has_changed(); };
	/// get the recorded URI path prefix for body_builder root hierarchy
    const chemin & get_prefix() const { return prefix; };

	/// provides a standard mean for inherited class to provide answer to requests
    virtual answer give_answer(const request & req) = 0;

protected:
	/// hook for inherited class, when the body builder object had its prefix changed
    virtual void prefix_has_changed() {};

private:
    chemin prefix;

};


#endif
