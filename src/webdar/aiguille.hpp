#ifndef AIGUILLE_HPP
#define AIGUILLE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"

class aiguille : public body_builder
{
public:
    aiguille() { mode = 0; };

    void set_mode(unsigned int m);

	// inherited from body_builder
    void inherited_give(body_builder *obj);
    void inherited_take_back(body_builder *obj) { unrecord_child(obj); };

    std::string get_body_part(const chemin & path,
			      const request & req);

protected:
	/// inherited from css grand-parent
        virtual void css_updated(bool inherited);

private:
    unsigned int mode;

};


#endif
