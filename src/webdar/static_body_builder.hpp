#ifndef STATIC_BODY_BUILDER_HPP
#define STATIC_BODY_BUILDER_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers

class static_body_builder
{
public:
    virtual ~static_body_builder() {};

    virtual std::string get_body_part() const = 0;
};

#endif
