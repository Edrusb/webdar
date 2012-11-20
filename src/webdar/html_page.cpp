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

void html_page::give(unsigned int x,
		     bool bold,
		     bool underscore,
		     bool italic,
		     const std::string & msg)
{
    html_text *tmp = new (nothrow) html_text(x);

    if(tmp == NULL)
	throw exception_memory();
    try
    {
	tmp->add_text(bold, underscore, italic, msg);
	body_builder::give(tmp);
    }
    catch(...)
    {
	delete tmp;
	throw;
    }
}


string html_page::get_body_part(const chemin & path,
				const request & req)
{
    string ret = "";

    ret += "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n";
    ret += "<html>\n<head>\n<title>\n";
    ret += x_title + "\n";
    ret += "</title>\n";
    if(redirect != "")
	ret += redirect + "\n";
    ret += "</head>\n<body ";
    ret += get_css_string() + ">\n";
    ret += get_body_part_from_all_children(path, req);
    ret += "\n</body>\n</html>\n";

    return ret;
}
