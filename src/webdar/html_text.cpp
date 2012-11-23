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


void html_text::add_text(unsigned int level, const std::string & text)
{
    string css_val = css_get_string();

    if(level == 0)
	if(css_val.empty())
	    txt += text + "\n";
	else
	    txt += "<div " + css_val + ">" + text + "</div>\n";
    else
    {
	string h = webdar_tools_convert_to_string(level);
	txt += "<h" + h + " " + css_val + ">" + text + "</h" + h + ">\n";
    }
}
