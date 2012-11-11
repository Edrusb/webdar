#ifndef HTML_TEXT_HPP
#define HTML_TEXT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "static_body_builder.hpp"


class html_text : public body_builder, public static_body_builder
{
public:
	/// constructor
	///
	/// header_size defines the header level (1 to ...) or zero for normal text
    html_text(unsigned int header_size) { size = header_size; };
    void add_text(bool bold, bool underscore, bool italic, const std::string & text);
    void add_nl() { txt += "<br />"; };
    void add_paragraph() { txt += "<p />"; };

    void clear(unsigned int header_size) { size = header_size; txt = ""; };

    virtual std::string get_body_part(const chemin & path,
				      const request & req) { return get_body_part(); };

    virtual std::string get_body_part() const;

private:
    unsigned int size;
    std::string txt;

};


#endif
