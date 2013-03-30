    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_comparison_fields.hpp"

using namespace std;


html_comparison_fields::html_comparison_fields() : html_form_select("Comparison field")
{
    add_choice("all", "All fields");
    add_choice("ignore_owner", "Ignore ownership");
    add_choice("mtime", "Ignore iwnership and permission");
    add_choice("inode_type", "ignore all but inode type");
    set_selected(0);
}

libdar::inode::comparison_fields html_comparison_fields::get_value() const
{
    switch(get_selected_num())
    {
    case 0:
	return libdar::inode::cf_all;
    case 1:
	return libdar::inode::cf_ignore_owner;
    case 2:
	return libdar::inode::cf_mtime;
    case 3:
	return libdar::inode::cf_inode_type;
    default:
	throw WEBDAR_BUG;
    }
}

void html_comparison_fields::set_value(libdar::inode::comparison_fields val)
{
    switch(val)
    {
    case libdar::inode::cf_all:
	set_selected(0);
	break;
    case libdar::inode::cf_ignore_owner:
	set_selected(1);
	break;
    case libdar::inode::cf_mtime:
	set_selected(2);
	break;
    case libdar::inode::cf_inode_type:
	set_selected(3);
	break;
    default:
	throw WEBDAR_BUG;
    }
}
