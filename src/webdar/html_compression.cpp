    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_compression.hpp"

using namespace std;

const string html_compression::changed = "html_compression_changed";

html_compression::html_compression(const string &title): html_form_select(title)
{
    add_choice("none", "None");
    add_choice("gzip", "Gzip");
    add_choice("bzip2", "Bzip2");
    add_choice("lzo", "lzo");
    set_selected(0);
    register_name(changed);
    record_actor_on_event(this, html_form_select::changed);
}

libdar::compression html_compression::get_value() const
{
    switch(get_selected_num())
    {
    case 0:
	return libdar::none;
    case 1:
	return libdar::gzip;
    case 2:
	return libdar::bzip2;
    case 3:
	return libdar::lzo;
    default:
	throw WEBDAR_BUG;
    }
}

void html_compression::set_value(libdar::compression val)
{
    switch(val)
    {
    case libdar::none:
	set_selected(0);
	break;
    case libdar::gzip:
	set_selected(1);
	break;
    case libdar::bzip2:
	set_selected(2);
	break;
    case libdar::lzo:
	set_selected(3);
	break;
    default:
	throw WEBDAR_BUG;
    }
}

void html_compression::on_event(const std::string & event_name)
{
	// when html_form_select changed propagate the change event to our own event name
    act(changed);
}
