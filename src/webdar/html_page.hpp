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
#include "html.hpp"
#include "exceptions.hpp"



class html_page : public html
{
public:
    html_page(const std::string & title) { x_title = title; };
    html_page(const html_page & ref) { throw WEBDAR_BUG; };
    const html_page & operator =(const html_page & ref) { throw WEBDAR_BUG; };
    ~html_page() { clear_body(); };

	/// adding contents to the body of the page
	///
	/// \param[in] obj the object is given to the html_page, it will be released by it
	/// \`note add_to_body can be called several time, the order of the object passed to it
	/// drive the order of the page composition.
    void add_to_body(const std::string & obj) { body_part.push_back(obj); };

    void set_refresh_redirection(unsigned int seconds, const std::string & url);

	/// drop all objects given so far using the add_to_body method
    void clear_body();

    virtual std::string display() const;

private:
    std::string x_title;
    std::string redirect;
    std::vector<std::string> body_part;
};


#endif
