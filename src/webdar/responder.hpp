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


    /////////////////////////////////////////////////////////////////////////////////////
    /// pure virtual class, base class for all classes that can generate an anwser to a request

class responder
{
public:
    virtual ~responder() {};

    virtual answer give_answer(const request & req) = 0;
};


#endif
