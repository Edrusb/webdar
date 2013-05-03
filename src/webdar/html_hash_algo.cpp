    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_hash_algo.hpp"

using namespace std;

html_hash_algo::html_hash_algo(const string & title):
    html_form_select(title)
{
    add_choice("none", "none");
    add_choice("md5", "md5");
    add_choice("sha1", "sha1");
    set_selected(0);
    register_name(changed);
    record_actor_on_event(this, html_form_select::changed);
}

libdar::hash_algo html_hash_algo::get_value() const
{
    switch(get_selected_num())
    {
    case 0:
	return libdar::hash_none;
    case 1:
	return libdar::hash_md5;
    case 2:
	return libdar::hash_sha1;
    default:
	throw WEBDAR_BUG;
    }
}

void html_hash_algo::set_value(libdar::hash_algo val)
{
    switch(val)
    {
    case libdar::hash_none:
	set_selected(0);
	break;
    case libdar::hash_md5:
	set_selected(1);
	break;
    case libdar::hash_sha1:
	set_selected(2);
	break;
    default:
	throw WEBDAR_BUG;
    }
}
