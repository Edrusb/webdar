    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_level.hpp"

using namespace std;


html_level::html_level()
{
    nxt = table.begin();
}


void html_level::give_static_html(const std::string & html)
{
    bundle tmp;

    tmp.obj = NULL;
    tmp.static_text = html;

    table.push_back(tmp);
    reset_read_next();
}

void html_level::clear()
{
    clear_and_delete_children();
    table.clear();
    reset_read_next();
}

void html_level::inherited_give(body_builder *obj)
{
    bundle tmp;

    if(obj == NULL)
	throw WEBDAR_BUG;
    (void)record_child(obj);
    tmp.obj = obj;
    table.push_back(tmp);
    reset_read_next();
}

bool html_level::read_next(bundle & bdl)
{
    if(nxt != table.end())
    {
	bdl = (*nxt);
	++nxt;
	return true;
    }
    else
	return false;
}

string html_level::get_body_part_from_children_as_a_block(const chemin & path,
							  const request & req)
{
    string ret = "";
    chemin sub_path = path;
    bundle bdl;

    if(sub_path.size() > 0)
	sub_path.pop_front();

    reset_read_next();
    while(read_next(bdl))
    {
	if(bdl.obj != NULL)
	    ret += bdl.obj->get_body_part(sub_path, req);
	else
	    ret += bdl.static_text;
    }

    return ret;
}
