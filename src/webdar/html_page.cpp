    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "webdar_tools.hpp"


    //
#include "html_page.hpp"

using namespace std;


void html_page::set_refresh_redirection(unsigned int seconds, const std::string & url)
{
    redirect = "<meta http-equiv=\"refresh\" content=\"";
    redirect += webdar_tools_convert_to_string(seconds);
    redirect += ";URL=" + url + "\">";
}


void html_page::clear_body()
{
/*
    vector<html *obj>::iterator it = body_part.begin();

    while(it != body_part.end())
    {
	if(*it != NULL)
	{
	    delete *it;
	    *it = NULL;
	}

	++it;
    }
*/

    body_part.clear();
}

string html_page::display() const
{
    string ret;
//    vector<html *obj>::const_iterator it = body_part.begin();
    vector<string>::const_iterator it = body_part.begin();

    ret = "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\">\n";
    ret += "<html>\n<head>\n<title>\n";
    ret += x_title + "\n";
    ret += "</title>\n";
    ret += redirect;
    ret += "\n</head>\n<body ";
    ret += get_css_string() + ">\n";
    while(it != body_part.end())
    {
/*
	if(*it == NULL)
	    throw WEBDAR_BUG;

	ret += it->display();
*/
	ret += *it;
	++it;
    }

    ret += "\n</body>\n</html>\n";

    return ret;
}
