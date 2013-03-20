#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "responder.hpp"
#include "saisie.hpp"

class user_interface : public responder
{
public:
	/// available event for that class
    static const std::string closing;

    user_interface();

	/// inherited from responder
    virtual answer give_answer(const request & req);

	/// inherited from actor
    virtual void on_event(const std::string & event_name);

protected:

	/// inherited from responder
    virtual void prefix_has_changed();

private:
    enum { config, listing, running } mode;

    saisie parametrage;
};

#endif
