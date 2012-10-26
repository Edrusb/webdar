#ifndef PARSER_HPP
#define PARSER_HPP

#include "exceptions.hpp"
#include "connexion.hpp"
#include "central_report.hpp"
#include "request.hpp"
#include "answer.hpp"

class parser
{
public:
	/// constructor
	///
	/// \param[in] input is the connexion to read data from
	/// \param[in] log where to log messages
	/// \note the connexion object passed to this becomes under the responsibility
	/// of this parser object and will be deleted when necessary. The log object is out of the responsibility of this object
	/// however it must survive this parser object.
    parser(connexion *input, central_report *log);

	/// avoid copy construction
    parser(const parser & ref): req(ref.clog) { throw WEBDAR_BUG; };

	/// avoid assignment
    const parser & operator = (const parser & ref) { throw WEBDAR_BUG; };

	/// destructor
    ~parser();

	/// provides visibility on the connection status
    connexion::status get_status() const { if(source == NULL) return connexion::not_connected; return source->get_status(); };

	/// get the next request URI
	///
	/// \param[out] uri the value of the next request uri
	/// \return false if not enough data is available to provide the uri
    bool get_next_request_uri(uri & val);

	/// provides the next request
    const request & get_request();

	/// modify the answer to conform to RFC 1945 before sending it
    void send_answer(answer & ans);


private:
    central_report *clog;      //< central report logging
    bool answered;             //< whether last request was answered or not
    connexion *source;         //< the connexion to the client
    request req;               //< value of the last request

    void valid_source() const { if(source == NULL || source->get_status() != connexion::connected) throw exception_range("socket disconnected"); };
    void checks_main(const request & req, answer & ans);
    void checks_webdar(const request & req, answer & ans);
    void checks_rfc1945(const request & req, answer & ans);
};


#endif
