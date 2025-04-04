/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "my_config.h"

    // C++ system header files
#include <string>
#include <map>
#include <memory>

    // webdar headers
#include "uri.hpp"
#include "webdar_tools.hpp"
#include "exceptions.hpp"
#include "tokens.hpp"
#include "central_report.hpp"
#include "connexion.hpp"

    /// class holding fields of an HTTP request (method, URI, header, cookies, and so on)

class request
{
public:
	/// The constructor
    request(const std::shared_ptr<central_report> & log) { clear(); if(log) clog = log; else throw WEBDAR_BUG; };
    request(const request & ref) = default;
    request(request && ref) noexcept = default;
    request & operator = (const request & ref) = default;
    request & operator = (request && ref) noexcept = default;
    ~request() = default;

	/// clear all fields of the request
    void clear();

	/// try reading just enough data in order to determine the uri of the next request
	///
	/// \note this method only fills the method and URI fields if data is available for that,
	/// in which case only, true is returned. get_method() and get_uri() can then be invoked
	/// to obtain the fields value.
    bool try_reading(proto_connexion & input);

	/// read the next request from input connexion
	///
	/// \note this method fills the internal fields (request line, header and body)
	/// \note may throw exceptions of type exception_range and exception_input.
	/// the first mean that the request could not be read completely, the caller
	/// must either not catch this type of exception, or reply by a bad-request answer
    	/// and close the connexion even in HTTP/1.1 because be could not completely read the request.
	/// Upon exception_input, the request could be read entirely, connexion can be maintained.
	/// The request code is to send in the answer is provided by this exception class.
    void read(proto_connexion & input);


	/// obtains the method of the read request
    const std::string & get_method() const { if(status < method_read) throw WEBDAR_BUG; return cached_method; };

	/// manually change the method of the request
    void change_method(const std::string & val) { if(status < method_read) throw WEBDAR_BUG; cached_method = val; };

	/// change POST request to a GET request, No modification for others
    void post_to_get() { if(status < method_read) throw WEBDAR_BUG; if(cached_method == "POST") cached_method = "GET"; };

	/// obtains the URI of the read request
    const uri & get_uri() const { if(status < uri_read) throw WEBDAR_BUG; return coordinates; };

	/// obtains the MAJOR version string of the read request
    int get_maj_version() const { if(status != completed) throw WEBDAR_BUG; return maj_vers; };

	/// obtains the MINOR version string of the read request
    int get_min_version() const { if(status != completed) throw WEBDAR_BUG; return min_vers; };

	/// obtain the body of the read request
    const std::string & get_body() const { if(status != completed) throw WEBDAR_BUG; return body; };

	/// obtain the body splitted in as list of attribute-value pair
	///
	/// \note this call can be used to analyse POST request's body in response to a form
	/// \note this call should only be used when the Content-Type is
	/// application/x-www-form-urlencoded, if not an exception is thrown
    std::map<std::string,std::string> get_body_form() const;

	/// manually add a cookie to the request (should be used exceptionally)
	///
	/// \note the semantic of a const method might sound strange here
	/// as we do modify the request object. Well, nothing's perfect... improvment
	/// may take place here, yes.
    void add_cookie(const std::string & key, const std::string & value) const;

	/// lookup for a cookie
    bool find_cookie(const std::string & key, std::string & value) const;

	/// raw request header header access
    bool find_attribute(const std::string & key, std::string & value) const;


	/// analyse body as a MIME multipart component (RFC 1521)

	/// \return the number of multipart found in the body of the request
	/// \note the request's header must have a header "Content-type: multipart/form-data; boundary=.....\r\n"
	/// \note if the request is not properly formated or is not a multipart one, an exception_range is thrown
    unsigned int get_multipart_number() const;

	/// obtains the headers of multiparts once get_multipart_number() has been executed

	/// \param[in] num the part number of the multipart in this request, first part is starting at index zero
	/// \return a map of key/value pair corresponding to the key/values pair found in the
	/// header the multipart number "num" found in the body
    std::map<troncon,troncon> get_header_of_multipart(unsigned int num) const; ///< first part is starting at index zero

	/// obtains the body of multiparts once get_multipart_number() has been executed

	/// \param[in] num the part number of the multipart in this request, first part is starting at index zero
	/// \return the document inclosed in the multipart number "num" of the body
    troncon get_body_of_multipart(unsigned int num) const;

	/// set the fields in consistent state to mimic a valid request

	/// \note used to convert body_builder class with static adopted child to static_body_builder class
    void fake_valid_request();

private:
    enum { init, method_read, uri_read, reading_all, completed } status;

    std::string cached_method;    //< method already read from the next request
    uri coordinates;              //< uri spit in fields
    unsigned int maj_vers;        //< HTTP major version of the last request received
    unsigned int min_vers;        //< HTTP minor version of the last request received
    std::map<std::string, std::string> attributes; //< request headers
    std::map<std::string, std::string> cookies;    //< request cookies
    std::string body;             //< request body if any
    std::shared_ptr<central_report> clog; //< central report logging

	/// multipart pointers
    typedef std::map<troncon, troncon> mp_header_map;

    mutable std::deque<mp_header_map> mp_headers;
    mutable std::deque<troncon> mp_body;

    void clear_multipart() { mp_headers.clear(); mp_body.clear(); };

	/// try reading the method and uri from the connexion
    bool read_method_uri(proto_connexion & input, bool blocking);

	// feed cookies fields from attributes and remove cookies from attributes
    void extract_cookies();

	/// split the string argument in two intergers to fields maj_vers and min_vers
    void set_version(const std::string & version);

    void add_attribute(const std::string & key, const std::string & value) { attributes.insert(std::pair<std::string, std::string>(webdar_tools_to_canonical_case(key), value)); };

    void drop_attribute(const std::string & key);

	/// true if next to read is end of line chars (CR LF)
    static bool is_empty_line(proto_connexion & input);

	/// returns what remains on the current lines
    static std::string up_to_eol(proto_connexion & input);

	/// returns what remains up to no data available on the socket
    static std::string up_to_eof(proto_connexion & input);

	/// skips the current line up to the next given argument.
	///
	/// \note Stops at end of line if the provided char is not found
    static void skip_over(proto_connexion & input, char a);

    static std::string up_to_length(proto_connexion & input, unsigned int length);

	/// drops all data up to and including the next end of line (CR LF).
    static void skip_line(proto_connexion & input);

	/// returns what remains on lines up to a real EOL (not LWS)

	/// \note LWS is a CR+LF followed by any number of space or tab.
	/// LWS allow a argument to be split over several lines. The spaces and tabs
	/// following a CR+LF are not part of the argument.
	/// this structure is used in header HTTP messages (RFC 1945)
    static std::string up_to_eol_with_LWS(proto_connexion & input);

	/// reads the next token from the socket
	/// \param[in] initial defines whether we can skip space to reach the token start
	/// \param[in] blocking defines whether the reading is blocking or not
	/// \param[out] token stores the token read
	/// \return true if a complete token could be read on the current line, false
	/// (in non blocking mode) if there is not enough data to define whether to token is
	/// complete or not.
	/// \note a token what defines the RFC 1945 at paragraph 2.2 "Basic Rules"
	/// \note the reading of a token does fails if no more token are available on the current
	/// line (CR, LF or CRLF met), true is returned in that case and token is set to an
	/// empty string
    static bool get_token(proto_connexion & input, bool initial, bool blocking, std::string & token);


	/// read the next word from the socket

	/// \param[in] initial defines whether we can skip space to reach the word start
	/// \param[in] blocking defines whether the reading is blocking or not
	/// \param[out] word stores the read word
	/// \return true if a complete word could be read on the current line, false
	/// (in non blocking mode) if there is not enough data to define wheter the word is
	/// complete or not.
	/// \note a word is composed of token and the following characters '/' ':' '=' '@' '?'
	/// \note the reading of a word fails if end of line is met, in that case true is returned
	/// and word is set to an empty string
    static bool get_word(proto_connexion & input, bool initial, bool blocking, std::string & word);

};

#endif
