#ifndef HTML_HASH_ALGO_HPP
#define HTML_HASH_ALGO_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "html_form_select.hpp"

class html_hash_algo : public html_form_select
{
public:
    static const std::string changed;

    html_hash_algo(const std::string & title);

    libdar::hash_algo get_value() const;
    void set_value(libdar::hash_algo val);

	/// actor indirect inheritance
    virtual void on_event(const std::string & event_name) { act(changed); };
};

#endif
