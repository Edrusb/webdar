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
#include "html_level.hpp"
#include "exceptions.hpp"


class html_page : public html_level
{
public:
    html_page(const std::string & title = "") { x_title = title; };

	/// change page title
    void set_title(const std::string & title) { x_title = title; };

	/// activate HTML redirection
	///
	/// \note if url is set to an empty string redirection is disabled
    void set_refresh_redirection(unsigned int seconds, const std::string & url);


	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

protected:
    std::string get_body_part_given_the_body(const chemin & path,
					     const request & req,
					     const std::string & body);

private:
    std::string x_title;
    std::string redirect;
};


#endif
