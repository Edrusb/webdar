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
	///
	/// \param[in] ref is the property to copy from
	/// \param|in] any_inheritance, if false, copy is done only if ref has inheritance set
	/// \param[in] even_if_set, if false, copy is not done if "this" is set
    void inherit_from(const css_property & ref, bool any_inheritance=false, bool even_if_set=false);

private:
    std::string value;
    bool inheritable;
    bool unset;

};


#endif
