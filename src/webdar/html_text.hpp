#ifndef HTML_TEXT_HPP
#define HTML_TEXT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html.hpp"


class html_text : public html
{
public:
	/// constructor
	///
	/// header_size defines the header level (1 to ...) or zero for normal text
    html_text(unsigned int header_size) { size = header_size; };
    void add_text(bool bold, bool underscore, bool italic, const std::string & text);
    void add_nl() { txt += "<br></br>"; };
    void add_paragraph() { txt += "<p></p>"; };

    void clear(unsigned int header_size) { size = header_size; txt = ""; };
    std::string display() const;

private:
    unsigned int size;
    std::string txt;

};


#endif
