#ifndef HTML_AIGUILLE_HPP
#define HTML_AIGUILLE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"

class html_aiguille : public body_builder
{
public:
    html_aiguille() { mode = 0; };

    void set_mode(unsigned int m);

	// inherited from body_builder
    void has_been_adopted(body_builder *obj);

    std::string get_body_part(const chemin & path,
			      const request & req);

protected:
	/// inherited from css grand-parent
        virtual void css_updated(bool inherited);

private:
    unsigned int mode;

};


#endif
