#ifndef PARSER_HPP
#define PARSER_HPP

#include "exceptions.hpp"
#include "connexion.hpp"
#include "req_ans.hpp"

class parser
{
public:
	/// constructor
	///
	/// \param[in] input is the connexion to read data from
	/// \note the connexion object passed to this becomes under the responsibility
	/// of the parser object
    parser(connexion *input);

	/// avoid copy construction
    parser(const parser & ref) { throw WEBDAR_BUG; };

	/// avoid assignment
    const parser & operator = (const parser & ref) { throw WEBDAR_BUG; };

	/// destructor
    ~parser();

	/// extract the next request from the connexion
	///
	/// \return the request found
	/// \note upon erorr, like badly formated data or unsupported HTTP version
	/// this method may throw an exception_range exception. In that situation
	/// the connexion is closed and this object is no more usable
    request get_next_request();

	/// send an answer
	///
	/// \param[in] ans is the answer to send to the client
	/// \note the answer may be modified to conforme with HTTP's RFCs
	/// like the addition to a "close" token
    void send_answer(const answer & ans);

private:
    connexion *source;         //< where to read data from
    unsigned buffer_size;      //< size of the buffer
    char *buffer;              //< temporary area used for parsing
    unsigned int already_read; //< amount of data already read
    unsigned int data_size;    //< total of data in buffer, already read or not
    bool answered;             //< false if a request is pending for an answer

	// first level read routines
    void fill_buffer();        //< manages to get data in buffer and set relative variables acordingly, if after that call no more data can be added this means that the connexion is broken (EOF).
    char read_one();           //< extracts one byte form the buffer
    char read_test_first();    //< gives the next char to be read, but do not remove it from the reading buffer / throw exception if not available
    char read_test_second();   //< gives the second next char to be read, but do not remove it from the reading buffer / throw exception if not available
    bool valid() const { return source != NULL && source->get_status() == connexion::connected; };

	// second level read routines
    bool is_empty_line();      //< true if next to read is end of line
    std::string get_token();   //< reads the next token from the socket: token is built upon any character that may take place in URI (as defined by RFC 2068)
    uri uri_split(const std::string & res); //< split URI arguments in uri datastructure
    void split_version(const std::string & version, unsigned int maj_vers, unsigned int min_vers); //< splits version in its maj.minor numbers
    void skip_line();          //< drops all data up to and including the next end of line (LF, eventually preceeded by CR).
    std::string up_to_eol();  //< returns what remains on the current lines
    std::string up_to_eof();  //< returns what remains up to no data available on the socket
    void skip_over(char a);   //< skips the current line up to the next given argument. Stops at end of line if provided char is not found
    std::string up_to_length(unsigned int length);
};


#endif
