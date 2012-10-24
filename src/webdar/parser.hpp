#ifndef PARSER_HPP
#define PARSER_HPP

#include "exceptions.hpp"
#include "connexion.hpp"
#include "req_ans.hpp"
#include "central_report.hpp"

class parser
{
public:
	/// constructor
	///
	/// \param[in] input is the connexion to read data from
	/// \note the connexion object passed to this becomes under the responsibility
	/// of the parser object
    parser(connexion *input, central_report *log);

	/// avoid copy construction
    parser(const parser & ref) { throw WEBDAR_BUG; };

	/// avoid assignment
    const parser & operator = (const parser & ref) { throw WEBDAR_BUG; };

	/// destructor
    ~parser();

	/// provides visibility on the connection status
    connexion::status get_status() const { if(source == NULL) return connexion::not_connected; return source->get_status(); };

	/// test whether a request is available for reading
    bool has_pending_request(uri & url);

	/// extract the next request from the connexion
	///
	/// \return the request found
	/// \note upon error, like badly formated data or unsupported HTTP version
	/// this method may throw an exception_range exception. In that situation
	/// the connexion is closed and this object is no more usable
    request get_next_request();

	/// send an answer
	///
	/// \param[in,out] ans is the answer to send to the client
	/// \note the answer may be modified to conforme with HTTP's RFCs
    void send_answer(answer & ans);

    unsigned int get_last_request_maj_version() const { return maj_vers; };
    unsigned int get_last_request_min_version() const { return min_vers; };

private:
    connexion *source;         //< where to read/write data from/to
    unsigned buffer_size;      //< size of the buffer
    char *buffer;              //< temporary area used for parsing, reading data FROM network
    unsigned int already_read; //< amount of data already read
    unsigned int data_size;    //< total of data in buffer, already read or not
    bool answered;             //< false if a request is pending for an answer
    request req;               //< cached request for sanity check when sending back the response
    unsigned int maj_vers;     //< HTTP major version of the last request received
    unsigned int min_vers;     //< HTTP minor version of the last request received
    central_report *clog;      //< central report logging
    bool close_asked_on_request; //< whether the last request contained a "close" token, in which case we must close the connection after the corresponding answer
    std::string cached_method; //< method already read from the next request
    bool cached_method_completed; //< true if a whole token could be read
    std::string cached_uri;    //< uri already read from the next request
    bool cached_uri_completed; //< true if a whole token could be read

	///////////////////////////////
	// first level read routines
	//  (extracting chars from socket, whatever they are)

    void fill_buffer(bool blocking);     //< manages to get data in buffer and set relative variables acordingly
    char read_one(bool blocking);        //< extracts one byte form the buffer / exception thrown if not available
    char read_test_first(bool blocking); //< gives the next char to be read, but do not remove it from the reading buffer / throw exception if not available
    char read_test_second(bool blocking);//< gives the second next char to be read, but do not remove it from the reading buffer / throw exception if not available (EOF)
    bool valid_source() const { return source != NULL && source->get_status() == connexion::connected; };

	///////////////////////////////
	// second level read routines
	//  (takes meanings from chars)

    bool is_empty_line();     //< true if next to read is end of line chars (CR LF)
    std::string up_to_eol();  //< returns what remains on the current lines
    std::string up_to_eof();  //< returns what remains up to no data available on the socket
    void skip_over(char a);   //< skips the current line up to the next given argument. Stops at end of line if provided char is not found
    std::string up_to_length(unsigned int length);
    void skip_line();         //< drops all data up to and including the next end of line (CR LF).

	/// returns what remains on lines up to a real EOL (not LWS)

	/// \note LWS is a CR+LF followed by any number of space or tab.
	/// LWS allow a argument to be split over several lines. The spaces and tabs
	/// following a CR+LF are not part of the argument.
	/// this structure is used in header HTTP messages (RFC 1945)
    std::string up_to_eol_with_LWS();

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
    bool get_token(bool initial, bool blocking, std::string & token);


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
    bool get_word(bool initial, bool blocking, std::string & word);

	///////////////////////////////
	// third level read routines
	//  (takes meaning from words)

    uri uri_split(const std::string & res); //< split URI arguments in uri datastructure
    void set_version(const std::string & version); //< updates the maj_vers and min_vers from string token
};


#endif
