
#include <libdar/user_interaction.hpp>

class web_interaction : public user_interaction, public req_ans
{
public:
    web_interaction(U_I num_line_buffer); //< max number of line of output to buffer
    ~web_interaction();

	// inherited from user_interaction
    virtual bool pause2();
    virtual std::string get_string(const std::string & message, bool echo);
    virtual void listing(const std::string & flag,
			 const std::string & perm,
			 const std::string & uid,
			 const std::string & gid,
			 const std::string & size,
			 const std::string & date,
			 const std::string & filename,
			 bool is_dir,
			 bool has_children);

    virtual web_interaction *clone() const;


	// interface for the web
    answer get_answer(const std::string & request_prefix, const request & req, const std::vector<av> & request_attributs);
	// this method provides consistent output with a field for last line of output
	// and a field for user interaction.

protected:
    virtual void inherited_warning(const std::string & message);

private:
    pthread_mutex_t pending_libdar; //< used to suspend libdar thread, waiting for user answer
    pthread_mutex_t output_buffer_lock; //< exclusive access to the output buffer
    std::deque<std::string> output_buffer; //< list of line to display
    U_I index_of_last_input; //< gives in output_buffer the number of new lines of output (or the offset of output already returned to the user)

};
