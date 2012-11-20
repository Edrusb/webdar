#ifndef CSS_PROPERTY_HPP
#define CSS_PROPERTY_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers

class css_property
{
public:
    css_property() { clear(); };

    void clear();
    void set_inheritance(bool val) { inheritable = val; };
    bool get_inheritance() const { return inheritable; };
    void set_value(const std::string & val);
    const std::string & get_value() const { return value; };
    bool is_unset() const { return unset; };

	/// copy the value from ref if unset and ref has inheritance set
    void inherit_from(const css_property & ref);

private:
    std::string value;
    bool inheritable;
    bool unset;

};


#endif
