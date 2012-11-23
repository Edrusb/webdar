    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "css_property.hpp"

using namespace std;

void css_property::clear()
{
    value = "";
    inheritable = false;
    unset = true;
}

void css_property::set_value(const string & val)
{
    value = val;
    unset = false;
}


void css_property::inherit_from(const css_property & ref,
				bool any_inheritance,
				bool even_if_set)
{
    if((even_if_set || is_unset())
       && (any_inheritance || ref.get_inheritance())
       && !ref.is_unset())
    {
	set_value(ref.get_value());
	set_inheritance(ref.get_inheritance());
    }
}
