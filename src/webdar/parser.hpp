#ifndef PARSER_HPP
#define PARSER_HPP

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
    unsigned char *buffer;     //< temporary area used for parsing
    unsigned int already_read; //< amount of data already read
    unsigned int data_end;     //< total of data in buffer, already read or not
    bool answered;             //< false if a request is pending for an answer

    bool valid() const { return source != NULL && source->get_status() == connexion::connected; };
    std::string read_token();


};


#endif
