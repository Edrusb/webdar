/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2022 Denis Corbin
//
// This file is part of Webdar
//
//  Webdar is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Webdar is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
//
//----
//  to contact the author: dar.linux@free.fr
/*********************************************************************/

#ifndef ANSWER_HPP
#define ANSWER_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <map>

    // webdar headers
#include "uri.hpp"
#include "webdar_tools.hpp"
#include "exceptions.hpp"
#include "proto_connexion.hpp"

    /// class answer provides easy means to set an HTTP answer and means to sent it back to a proto_connexion object

    /// \note the main consumers of class answer are objects from responder class and its inherited classes

class answer
{
public:
        /// class constructor
    answer() { clear(); };

        /// copy constructor
    answer(const answer & ref) { copy_from(ref); };

        /// move constructor
    answer(answer && ref) noexcept = default;

        /// assignment copy operator
    answer & operator = (const answer & ref) { copy_from(ref); return *this; };

        /// assignment move operator
    answer & operator = (answer && ref) noexcept = default;

        /// destructor
    ~answer() = default;


        /////// SETTING THE OBJECT

        /// clear all information from the object
    void clear();

        /// set answer status code and reason [MANDATORY]
    void set_status(unsigned int status_code) { status = status_code; };

        /// set reason [optional]
    void set_reason(const std::string & reason_phrase) { reason = reason_phrase; };

        /// set version info [MANDATORY]
    void set_version(unsigned int maj, unsigned int min) { maj_vers = maj; min_vers = min; };

        /// add cookie to the answer [optional]
    void add_cookie(const std::string & key, const std::string & value);

        /// adds the  body to the answer [optional]

        /// \note this also set Content-Length accordingly
    void add_body(const std::string & key);

        /// removes the body keeping header untouched (Content-Lenght in particular)
    void drop_body_keep_header() { body = ""; };

        /// set a given attribute to the HTTP header
    void set_attribute(const std::string & key, const std::string & value) { attributes[webdar_tools_to_canonical_case(key)] = value; };

        /// add an attribute to a possibly already existing message header
        ///
        /// \note according to RFC1945:
        /// "Multiple HTTP-header fields with the same field-name may be present
        /// in a message if and only if the entire field-value for that header
        /// field is defined as a comma-separated list [i.e., #(values)]. It must
        /// be possible to combine the multiple header fields into one "field-
        /// name: field-value" pair, without changing the semantics of the
        ///  message, by appending each subsequent field-value to the first, each
        /// separated by a comma.
        ///  Multiple HTTP-header fields with the same field-name may be present
        /// in a message if and only if the entire field-value for that header
        /// field is defined as a comma-separated list [i.e., #(values)]. It must
        /// be possible to combine the multiple header fields into one "field-
        /// name: field-value" pair, without changing the semantics of the
        /// message, by appending each subsequent field-value to the first, each
        /// separated by a comma."
        /// This is the way add_attribute_member behaves. If the given key already
        /// exists, the given value is added to the existing value to form a new CSV f
    void add_attribute_member(const std::string & key, const std::string & value);


        /////// VALIDATING THE OBJECT

        /// whether the minimal parameters have been set
    bool is_valid() const;



        /////// GETTING INFORMATIONS ABOUT THE OBJECT

        /// get answer status code and reason
    unsigned int get_status_code() const { return status; };

        /// get reason
    const std::string get_reason() const { return reason; };

        /// get integer part of the version info
    unsigned int get_maj_version() const { return maj_vers; };

        /// get decimal part of the version info
    unsigned int get_min_version() const { return min_vers; };

        /// get the current body of the answer
    const std::string get_body() const { return body; };

        /// retrieve the value of an attribute of the HTTP answer
        ///
        /// \param[in] key is the key's attribute to look for
        /// \param[out] value is the associated value of the attribute if such attribute exists
        /// \return true if the requested attribute has been found in this request
    bool find_attribute(const std::string & key, std::string & value) const;


        /////// SERIALIZING THE OBJECT TO AN EXISTING CONNECTION

        /// send the answer
    void write(proto_connexion & output);

private:
    unsigned int status;       ///< the HTTP status the answer should return
    std::string reason;        ///< the HTTP reason the answer should return
    unsigned int maj_vers;     ///< the HTTP version of the answer (in HTTP/1.0 maj_vers is 1)
    unsigned int min_vers;     ///< the HTTP decimal version of the answer (in HTTP/1.0 min_vers is 0)
    std::map<std::string, std::string> attributes; ///< http answer attributes like cookies
    std::string body;          ///< the HTTP body (HTML header + HTML Body) of the HTTP answer

        /// field used to sequentially read the map of attributes
    mutable std::map<std::string, std::string>::const_iterator next_read;

        /// reset the read_next_attribute to the beginning of the list
    void reset_read_next_attribute() const;

        /// reads the next attributes
        ///
        /// \param[out] key key of the next attribute
        /// \param[out] value value of that attribute
        /// \return true if a next attribute has been else, key and value are not set
    bool read_next_attribute(std::string & key, std::string & value) const;

        /// used in copy constructor and copy operators
    void copy_from(const answer & ref);

};

#endif
