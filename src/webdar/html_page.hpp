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
    html_page(const std::string & title = "") { x_title = title; };

	/// change page title
    void set_title(const std::string & title) { x_title = title; };

	/// activate HTML redirection
	///
	/// \note if url is set to an empty string redirection is disabled
    void set_refresh_redirection(unsigned int seconds, const std::string & url);


	/// add static text to the page
    void give(unsigned int x,
	      bool bold,
	      bool underscore,
	      bool italic,
	      const std::string & msg);


	/// inherited from body_builder
    void take_back(body_builder *obj) { unrecord_child(obj); };


	/// destroy all given objects
    void clear() { clear_and_delete_children(); };

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

protected:
	/// inherited from body_builder
    void inherited_give(body_builder *obj) { (void)record_child(obj); };



private:
    std::string x_title;
    std::string redirect;
};


#endif
