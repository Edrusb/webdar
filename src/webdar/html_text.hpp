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
	/// adding text
	///
	/// \param[in] level 1 for <h1> etc. and zero for normal text
    void add_text(unsigned int level, const std::string & text);
    void add_nl() { txt += "<br />"; };
    void add_paragraph() { txt += "<p />"; };

    void clear() { txt = ""; };

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req) { return get_body_part(); };

	/// inherited from static_body_builder
    virtual std::string get_body_part() const { return txt; };

private:
    std::string txt;

};


#endif
