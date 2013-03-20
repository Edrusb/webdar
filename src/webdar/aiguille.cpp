    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "aiguille.hpp"

using namespace std;

void aiguille::set_mode(unsigned int m)
{
    if(m < size())
	mode = m;
    else
	throw WEBDAR_BUG;
}

void aiguille::has_been_adopted(body_builder *obj)
{
    if(obj == NULL)
	throw WEBDAR_BUG;
    obj->css_inherit_from(*this, true, false);
}

string aiguille::get_body_part(const chemin & path,
			      const request & req)
{
    if(mode < size())
    {
	if((*this)[mode] == NULL)
	    throw WEBDAR_BUG;
	else
	{
	    chemin sub_path = path;

	    if(sub_path.size() > 1)
		sub_path.pop_front();


	    return (*this)[mode]->get_body_part(sub_path, req);
	}
    }
    else
	throw WEBDAR_BUG;
}


void aiguille::css_updated(bool inherited)
{
    unsigned int s = size();

    for(unsigned int i = 0; i < s; ++i)
    {
	if((*this)[i] == NULL)
	    throw WEBDAR_BUG;
	(*this)[i]->css_inherit_from(*this, true, false);
    }
}
