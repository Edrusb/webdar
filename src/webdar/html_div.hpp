#ifndef HTML_DIV_HPP
#define HTML_DIV_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_level.hpp"

    /// class html_div is the implementation of <div> </div>
    ///
    /// use the body_builder::give() and html_level::give_static_html() to add
    /// data into the <div> </div>. Apply css_*() methods to it

class html_div : public html_level
{
public:

    virtual std::string get_body_part(const chemin & path,
				      const request & req);

};

#endif
