#ifndef DATE_HPP
#define DATE_HPP

    // C system header files
extern "C"
{
#include <time.h>
#include <ctype.h>
}

    // C++ system header files
#include <string>

    // webdar headers


class date
{
public:
	/// constructor, date'value is "now"
    date();

	/// constructor, date is given in one of the following format
	///
	/// \note RFC 1123, RFC 1036 or ANSI C's asctime() formats
    date(const std::string & when);

	/// returns date in RFC 1123 format
    std::string get_canonical_format() const;

    bool operator < (const date & ref) const;
    bool operator <= (const date & ref) const { return *this < ref || *this == ref; };
    bool operator == (const date & ref) const;
private:
    struct tm val;  // date is stored as UTC

};


#endif
