    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "webdar_tools.hpp"


    //
#include "html_text.hpp"

using namespace std;


void html_text::add_text(bool bold, bool underlined, bool italic, const std::string & text)
{
    string css = get_css_string();

    if(css != "")
	txt += "<div " + css + ">";
    if(bold)
	txt += "<b>";
    if(underlined)
	txt += "<u>";
    if(italic)
	txt += "<i>";
    txt += text;
    if(italic)
	txt += "</i>";
    if(underlined)
	txt += "</u>";
    if(bold)
	txt += "</b>";
    if(css != "")
	txt += "</div>";
}

string html_text::get_body_part() const
{
    if(size == 0)
	return txt;
    else
    {
	string h = webdar_tools_convert_to_string(size);

	return "<h" + h + ">" + txt + "</h" + h + ">";
    }
}
