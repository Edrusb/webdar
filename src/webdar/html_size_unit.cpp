    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_size_unit.hpp"

using namespace std;

html_size_unit::html_size_unit() : html_form_select(""),
				   SI_mode("")
{
    SI_mode.add_choice("SI", "SI");
    SI_mode.add_choice("bin", "binary");
    SI_mode.set_selected(0);
    SI_mode.record_actor_on_event(this, html_form_select::changed);
    set_fields();
}

libdar::infinint html_size_unit::get_value() const
{
    libdar::infinint ret;
    libdar::infinint base;

    switch(SI_mode.get_selected_num())
    {
    case 0:
	base = 1000;
	break;
    case 1:
	base = 1024;
	break;
    default:
	throw WEBDAR_BUG;
    }

    ret = base.power(get_selected_num());

    return ret;
}

void html_size_unit::on_event(const std::string & event_name)
{
    set_fields();
}

string html_size_unit::get_body_part(const chemin & path,
				     const request & req)
{
    return html_form_select::get_body_part(path, req) + SI_mode.get_body_part(path, req);
}


void html_size_unit::set_fields()
{
    switch(SI_mode.get_selected_num())
    {
    case 0:
	clear();
	add_choice("octet", "o");
	add_choice("kilo", "ko");
	add_choice("mega", "Mo");
	add_choice("giga", "Go");
	add_choice("tera", "To");
	add_choice("peta", "Po");
	add_choice("exa", "Eo");
	add_choice("zetta", "Zo");
	add_choice("yotta", "Yo");
	break;
    case 1:
	clear();
	add_choice("octet", "o");
	add_choice("kilo", "kio");
	add_choice("mega", "Mio");
	add_choice("giga", "Gio");
	add_choice("tera", "Tio");
	add_choice("peta", "Pio");
	add_choice("exa", "Eio");
	add_choice("zetta", "Zio");
	add_choice("yotta", "Yio");
	break;
    default:
	throw WEBDAR_BUG;
    }
}
