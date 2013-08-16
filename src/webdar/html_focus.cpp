    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers


    //
#include "html_focus.hpp"

using namespace std;

void html_focus::given_for_temporary_adoption(body_builder *obj)
{
    if(adopted != NULL)
	foresake(adopted);
    adopted = obj;
    adopt(obj);
}

string html_focus::get_body_part(const chemin & path,
				 const request & req)
{
    string ret;

    if(get_visible())
	ret = get_body_part_from_all_children(path, req);

    if(!get_next_visible())
	ret = "";
    ack_visible();

    return ret;
}
