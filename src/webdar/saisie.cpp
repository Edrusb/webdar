    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "exceptions.hpp"
#include "html_text.hpp"
#include "html_form.hpp"
#include "html_form_input.hpp"
    //
#include "saisie.hpp"

using namespace std;

saisie::saisie()
{
    menu *tmp_choice = NULL;
    aiguille *tmp_select = NULL;
    html_url *tmp_sessions = NULL;
    html_div *tmp_sep = NULL;

	// locally used variables
    html_text text;

    choice = NULL;
    select = NULL;
    sessions = NULL;

    try
    {
	tmp_choice = new menu();
	tmp_select = new aiguille();
	tmp_sessions = new html_url("/", "Other sessions");
	tmp_sep = new html_div();

	if(tmp_choice == NULL
	   || tmp_select == NULL
	   || tmp_sessions == NULL
	   || tmp_sep == NULL)
	    throw exception_memory();

	choice = tmp_choice;
	select = tmp_select;
	sessions = tmp_sessions;

	    // configuration of "choice"
	tmp_choice->add_entry("about", "Main Page");
	tmp_choice->add_entry("restore", "Restoration");
	tmp_choice->add_entry("compare", "Comparison");
	tmp_choice->add_entry("test", "Testing");
	tmp_choice->add_entry("list", "Listing");
	tmp_choice->add_entry("create", "Creation");
	tmp_choice->add_entry("isolate", "Isolation");
	tmp_choice->add_entry("merge", "Merging");
	tmp_choice->add_entry("filters", "Filters");
	tmp_choice->add_entry("sep", "");
	tmp_choice->add_entry("close", "Close Session");
	tmp_choice->css_float(css::fl_left);
	give(tmp_choice);
	tmp_choice = NULL;

	    // top middle conditionnally shown archive name
	set_show_archive();
	give(show_archive);

	    // configuration of tmp_session
	tmp_sessions->css_float(css::fl_left);
	tmp_sessions->css_float_clear(css::fc_left);
	give(tmp_sessions);
	tmp_sessions = NULL;

	    // configuration of the sep subpage
	text.clear();
	text.add_text(1, "BUG: THIS separation PAGE SHOULD NEVER SHOW");
	tmp_sep->give_static_html(text.get_body_part());

	set_about();
	tmp_select->give(about);
	set_restore();
	tmp_select->give(restore);
	set_compare();
	tmp_select->give(compare);
	set_test();
	tmp_select->give(test);
	set_list();
	tmp_select->give(list);
	set_create();
	tmp_select->give(create);
	set_isolate();
	tmp_select->give(isolate);
	set_merge();
	tmp_select->give(merge);
	set_filters();
	tmp_select->give(filters);
	tmp_select->give(tmp_sep);
	tmp_sep = NULL;
	set_close();
	tmp_select->give(close);

	    // configuration of "select"
	tmp_select->css_margin_left("9.4em");
	tmp_select->set_mode(0);
	give(tmp_select);
	tmp_select = NULL;
    }
    catch(...)
    {
	if(tmp_choice != NULL)
	    delete tmp_choice;
	if(tmp_select != NULL)
	    delete tmp_select;
	if(tmp_sessions != NULL)
	    delete tmp_sessions;
	if(tmp_sep != NULL)
	    delete tmp_sep;
	throw;
    }
}

string saisie::get_body_part(const chemin & path,
			     const request & req)
{
    chemin sub_path = path;

    if(!sub_path.empty())
	sub_path.pop_front();

    if(choice == NULL)
	throw WEBDAR_BUG;
    if(select == NULL)
	throw WEBDAR_BUG;
    if(show_archive == NULL)
	throw WEBDAR_BUG;

    (void)choice->get_body_part(sub_path, req);
    set_title(string("WEBDAR - ") + choice->get_current_label());
    switch(choice->get_current_mode())
    {
    case 0:
    case 8:
    case 9:
    case 10:
	show_archive->set_mode(0);
	break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
	show_archive->set_mode(1);
	break;
    default:
	throw WEBDAR_BUG;
    }
    select->set_mode(choice->get_current_mode());

    return html_page::get_body_part(path, req);
}

void saisie::set_show_archive()
{
    show_archive = new (nothrow) aiguille();
    if(show_archive == NULL)
	throw exception_memory();

    try
    {
	html_text *tmp_void = new (nothrow) html_text();
	if(tmp_void == NULL)
	    throw exception_memory();
	show_archive->give(tmp_void);

	html_form *tmp_form = new (nothrow) html_form("update");
	if(tmp_form == NULL)
	    throw exception_memory();
	try
	{
	    archive = new (nothrow) html_form_input("Archive Basename",
						    html_form_input::text, "", 50);
	    if(archive == NULL)
		throw exception_memory();
	    tmp_form->give(archive);
	    show_archive->give(tmp_form);
	}
	catch(...)
	{
	    if(tmp_form != NULL)
		delete tmp_form;
	    throw;
	}
    }
    catch(...)
    {
	delete show_archive;
	show_archive = NULL;
	throw;
    }
}

void saisie::set_about()
{
    html_text text;

    about = new html_div();
    if(about == NULL)
	throw exception_memory();

    try
    {
	text.add_text(1,"WEBDAR");
	text.add_paragraph();
	text.add_text(4,string("version ") + WEBDAR_VERSION);
	text.add_paragraph();
	text.add_text(0, "by Denis CORBIN");
	about->give_static_html(text.get_body_part());
    }
    catch(...)
    {
	delete about;
	about = NULL;
	throw;
    }
}

void saisie::set_restore()
{
    html_text text;

    restore = new (nothrow) html_div();
    if(restore == NULL)
	throw exception_memory();

    try
    {
	text.clear();
	text.add_text(1, "restoration page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	restore->give_static_html(text.get_body_part());
    }
    catch(...)
    {
	delete restore;
	restore = NULL;
	throw;
    }
}

void saisie::set_compare()
{
    html_text text;

    compare = new (nothrow) html_div();
    if(compare == NULL)
	throw exception_memory();

    try
    {
	text.clear();
	text.add_text(1, "comparison page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	compare->give_static_html(text.get_body_part());
    }
    catch(...)
    {
	delete compare;
	compare = NULL;
	throw;
    }
}

void saisie::set_test()
{
    html_text text;

    test = new (nothrow) html_div();
    if(test == NULL)
	throw exception_memory();

    try
    {
	text.clear();
	text.add_text(1, "test page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	test->give_static_html(text.get_body_part());
    }
    catch(...)
    {
	delete test;
	test = NULL;
	throw;
    }
}

void saisie::set_list()
{
    list = new (nothrow) options_listing();
    if(list == NULL)
	throw exception_memory();
}

void saisie::set_create()
{
    html_text text;

    create = new (nothrow) html_div();
    if(create == NULL)
	throw exception_memory();

    try
    {
	text.clear();
	text.add_text(1, "create page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	create->give_static_html(text.get_body_part());
    }
    catch(...)
    {
	delete create;
	create = NULL;
	throw;
    }
}

void saisie::set_isolate()
{
    html_text text;

    isolate = new (nothrow) html_div();
    if(isolate == NULL)
	throw exception_memory();

    try
    {
	text.clear();
	text.add_text(1, "isolation page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	isolate->give_static_html(text.get_body_part());
    }
    catch(...)
    {
	delete isolate;
	isolate = NULL;
	throw;
    }
}

void saisie::set_merge()
{
    html_text text;

    merge = new (nothrow) html_div();
    if(merge == NULL)
	throw exception_memory();

    try
    {
	text.clear();
	text.add_text(1, "merge page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	merge->give_static_html(text.get_body_part());
    }
    catch(...)
    {
	delete merge;
	merge = NULL;
	throw;
    }
}

void saisie::set_filters()
{
    html_text text;

    filters = new (nothrow) html_div();
    if(filters == NULL)
	throw exception_memory();

    try
    {
	text.clear();
	text.add_text(1, "filters page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	filters->give_static_html(text.get_body_part());
    }
    catch(...)
    {
	delete filters;
	filters = NULL;
	throw;
    }
}

void saisie::set_close()
{
    html_text text;

    close = new (nothrow) html_div();
    if(close == NULL)
	throw exception_memory();

    try
    {
	text.clear();
	text.add_text(1, "session close page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	close->give_static_html(text.get_body_part());
    }
    catch(...)
    {
	delete close;
	close = NULL;
	throw;
    }
}
