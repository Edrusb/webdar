    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "webdar_tools.hpp"
#include "html_text.hpp"

    //
#include "html_page.hpp"

using namespace std;


void html_page::set_refresh_redirection(unsigned int seconds, const std::string & url)
{
    if(url != "")
    {
	redirect = "<meta http-equiv=\"refresh\" content=\"";
	redirect += webdar_tools_convert_to_string(seconds);
	redirect += ";URL=" + url + "\">";
    }
    else
	redirect = "";
}


string html_page::get_body_part(const chemin & path,
				const request & req)
{
    return get_body_part_given_the_body(path, req, get_body_part_from_children_as_a_block(path, req));
}

string html_page::get_body_part_given_the_body(const chemin & path,
					       const request & req,
					       const std::string & body)
{
    string ret = "";

    ret += "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n";
    ret += "<html>\n<head>\n<title>\n";
    ret += x_title + "\n";
    ret += "</title>\n";
    if(redirect != "")
	ret += redirect + "\n";
    ret += "</head>\n<body ";
    ret += css_get_string() + ">\n";
    ret += body;
    ret += "</body>\n</html>\n";

    return ret;
}

