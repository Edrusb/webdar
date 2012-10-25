#ifndef ANSWER_HPP
#define ANSWER_HPP

    // C++ system header files
#include <string>
#include <map>

    // webdar headers
#include "uri.hpp"
#include "webdar_tools.hpp"
#include "exceptions.hpp"
#include "connexion.hpp"

class answer
{
public:
    answer() { clear(); };

	/// set/get answer status code and reason
    void set_status(unsigned int status_code) { status = status_code; };
    void set_reason(const std::string & reason_phrase) { reason = reason_phrase; };
    void set_version(unsigned int maj, unsigned int min) { maj_vers = maj; min_vers = min; };
    unsigned int get_status_code() const { return status; };
    const std::string get_reason() const { return reason; };
    unsigned int get_maj_version() const { return maj_vers; };
    unsigned int get_min_version() const { return min_vers; };

	/// adds or remplace a given attribute  the HTTP answer
    void add_attribute(const std::string & key, const std::string & value) { attributes.insert(std::pair<std::string, std::string>(webdar_tools_to_lowercase(key), value)); };

	/// retrieve the value of an attribute of the HTTP answer
	///
	/// \param[in] key is the key's attribute to look for
	/// \param[out] value is the associated value of the attribute if such attribute exists
	/// \return true if the requested attribute has been found in this request
    bool find_attribute(const std::string & key, std::string & value) const;

	/// reset the read_next_attribute to the beginning of the list
    void reset_read_next_attribute() const;

	/// reads the next attributes
	///
	/// \param[out] key key of the next attribute
	/// \param[out] value value of that attribute
	/// \return true if a next attribute has been else, key and value are not set
    bool read_next_attribute(std::string & key, std::string & value) const;

	/// add cookie to the answer, that is a special attribute
    void add_cookie(const std::string & key, std::string & value);

	/// adds the  body to the answer
    void add_body(const std::string & key) { body = key; };
    const std::string get_body() const { return body; };

	/// clear all information from the object
    void clear() { status = maj_vers = min_vers = 0; reason = ""; attributes.clear(); body = ""; next_read = attributes.begin(); };

	/// whether the minimal parameters have been set
    bool is_empty() const { return status == 0; };


	/// send the answer
    void write(connexion & output);

private:
    unsigned int status;
    std::string reason;
    unsigned int maj_vers;
    unsigned int min_vers;
    std::multimap<std::string, std::string> attributes;
    std::string body;
    std::multimap<std::string, std::string>::const_iterator next_read;

};

#endif
