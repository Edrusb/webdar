/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2022 Denis Corbin
//
// This file is part of Webdar
//
//  Webdar is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Webdar is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
//
//----
//  to contact the author: dar.linux@free.fr
/*********************************************************************/

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


string html_page::inherited_get_body_part(const chemin & path,
					  const request & req)
{
    return get_body_part_given_the_body(path, req, get_body_part_from_children_as_a_block(path, req));
}

string html_page::get_body_part_given_the_body(const chemin & path,
					       const request & req,
					       const std::string & body)
{
    string ret = "";
    string aux;
    unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    ret += "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n";
    ret += "<html>\n<head>\n";
    if(csslib->size() > 0)
	ret += "<style>\n" + csslib->get_html_class_definitions() + "</style>\n";
    ret += "<title>\n";
    ret += x_title + "\n";
    ret += "</title>\n";
    ret += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    ret += "<meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">\n";

    if(redirect != "")
	ret += redirect + "\n";

    ret += "</head>\n<body";

    aux = get_css_classes();
    if(! aux.empty())
	ret += " " + aux;
    ret += ">\n";

    ret += body;
    ret += "</body>\n</html>\n";

    return ret;
}
