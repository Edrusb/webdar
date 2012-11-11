#ifndef HTML_PAGE_HPP
#define HTML_PAGE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <vector>

    // webdar headers
#include "body_builder.hpp"
#include "exceptions.hpp"



class html_page : public body_builder
{
public:
    html_page() { set_title(""); };
    html_page(const std::string & title) { set_title(title); };

	/// change page title
    void set_title(const std::string & title) { x_title = title; };

	/// activate HTML redirection
	///
	/// \note if url is set to an empty string redirection is disabled
    void set_refresh_redirection(unsigned int seconds, const std::string & url);


	/// add content to the page body
	///
	/// \note no transfer of ownerhip is done concerning the object which address is given
	/// the caller must manage for that the given object exists when get_body_part of this
	/// object is called
    void add_content(body_builder *obj);

	/// remove content from the page body
    void remove_content(body_builder *obj);

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

private:
    std::string x_title;
    std::string redirect;
};


#endif
