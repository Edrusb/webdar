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

    //
#include <unistd.h>


    // libraries header files
#include <dar/libdar.hpp>
#include <libthreadar/libthreadar.hpp>

#include "html_page.hpp"
#include "html_menu.hpp"
#include "html_table.hpp"
#include "html_div.hpp"
#include "css.hpp"
#include "pipe_connexion.hpp"

using namespace std;


int main(int argc, char *argv[])
{
    const char *reqstring = "GET / HTTP/1.1\r\n\r\n";

    try
    {
	int pipeA[2], pipeB[2];
	shared_ptr<central_report> rep = make_shared<central_report_stdout>(debug);
	chemin chem("/chemin");

	if(pipe(pipeA) < 0)
	    throw exception_system("creating anonymous pipeA", errno);
	if(pipe(pipeB) < 0)
	    throw exception_system("creating anonymous pipeB", errno);

	pipe_connexion cnx(pipeA[0], pipeB[1], "1.2.3.4", 7);
	write(pipeA[1], reqstring, strlen(reqstring));
	request req(rep);
	req.read(cnx);

	html_page page("mon titre");
	html_menu menu;
	html_div div;
	html_table table(4);

	page.adopt(&div);
	div.adopt(&menu);
	page.adopt(&table);

	menu.add_entry("coucou", "coucou tag");
	menu.add_entry("coucou2", "coucou2 tag");
	menu.add_entry("", "");
	menu.add_entry("coucou3", "coucou3 tag");
	menu.set_current_mode(1);
	table.adopt_static_html("col1");
	table.adopt_static_html("col2");
	table.adopt_static_html("col3");
	table.adopt_static_html("col4");
	for(unsigned int i = 1; i < 5; ++i)
	{
	    ostringstream conv;
	    conv << i;
	    table.adopt_static_html(string("val ") + conv.str());
	}

	css cm;
	string namem = "class_tutu";
	cm.css_float(css::fl_left);
	div.add_css_class(namem);
	page.define_css_class_in_library(namem, cm);

	css ct;
	string namet = "class_toto";
//	ct.css_width("30%", false);
//	ct.css_float(css::fl_right);
	table.add_css_class(namet);
	page.define_css_class_in_library(namet, ct);

	cout << page.get_body_part(chem, req) << endl;
    }
    catch(exception_base & e)
    {
	cout << "EXCEPTION ------------" << endl;
	cout << e.get_message() << endl;
	cout << "EXCEPTION ------------" << endl;
    }
}
