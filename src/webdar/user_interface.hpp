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
    user_interface() { mode = config; };

	// inherited from responder
    virtual answer give_answer(const request & req);

protected:

	/// inherited from responder
    virtual void prefix_has_changed();

private:
    enum { config, running } mode;

    saisie parametrage;
};

#endif
