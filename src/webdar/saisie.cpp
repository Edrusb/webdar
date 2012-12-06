    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "exceptions.hpp"
#include "html_text.hpp"

    //
#include "saisie.hpp"

using namespace std;

saisie::saisie()
{
    menu *tmp_choice = NULL;
    aiguille *tmp_select = NULL;
    html_url *tmp_sessions = NULL;
    html_div *tmp_about = NULL;
    html_div *tmp_restore = NULL;
    html_div *tmp_compare = NULL;
    html_div *tmp_test = NULL;
    options_listing *tmp_list = NULL;
    html_div *tmp_create = NULL;
    html_div *tmp_isolate = NULL;
    html_div *tmp_merge = NULL;
    html_div *tmp_filters = NULL;
    html_div *tmp_sep = NULL;
    html_div *tmp_close = NULL;

	// locally used variables
    html_text text;

    choice = NULL;
    select = NULL;
    sessions = NULL;
    about = NULL;
    restore = NULL;
    compare = NULL;
    test = NULL;
    list = NULL;
    create = NULL;
    isolate = NULL;
    merge = NULL;
    filters = NULL;
    sep = NULL;
    close = NULL;

    try
    {
	tmp_choice = new menu();
	tmp_select = new aiguille();
	tmp_sessions = new html_url("/", "Other sessions");
	    //
	tmp_about = new html_div();
	tmp_restore = new html_div();
	tmp_compare = new html_div();
	tmp_test = new html_div();
	tmp_list = new options_listing();
	tmp_create = new html_div();
	tmp_isolate = new html_div();
	tmp_merge = new html_div();
	tmp_filters = new html_div();
	tmp_sep = new html_div();
	tmp_close = new html_div();

	if(tmp_choice == NULL
	   || tmp_select == NULL
	   || tmp_sessions == NULL
	   || tmp_about == NULL
	   || tmp_restore == NULL
	   || tmp_compare == NULL
	   || tmp_test == NULL
	   || tmp_list == NULL
	   || tmp_create == NULL
	   || tmp_isolate == NULL
	   || tmp_merge == NULL
	   || tmp_filters == NULL
	   || tmp_sep == NULL
	   || tmp_close == NULL)
	    throw exception_memory();

	choice = tmp_choice;
	select = tmp_select;
	sessions = tmp_sessions;
	about = tmp_about;
	restore = tmp_restore;
	compare = tmp_compare;
	test = tmp_test;
	list = tmp_list;
	create = tmp_create;
	isolate = tmp_isolate;
	merge = tmp_merge;
	filters = tmp_filters;
	sep = tmp_sep;
	close = tmp_close;

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

	    // configuration of tmp_session
	tmp_sessions->css_float(css::fl_left);
	tmp_sessions->css_float_clear(css::fc_left);
	give(tmp_sessions);
	tmp_sessions = NULL;

	    // configuration of the about sub page
	text.add_text(1,"WEBDAR");
	text.add_paragraph();
	text.add_text(4,string("version ") + WEBDAR_VERSION);
	text.add_paragraph();
	text.add_text(0, "by Denis CORBIN");
	tmp_about->give_static_html(text.get_body_part());
	tmp_select->give(tmp_about);
	tmp_about = NULL;

	    // configuration of the restore subpage
	text.clear();
	text.add_text(1, "restoration page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	tmp_restore->give_static_html(text.get_body_part());
	tmp_select->give(tmp_restore);
	tmp_restore = NULL;

	    // configuration of the compare subpage
	text.clear();
	text.add_text(1, "comparison page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	tmp_compare->give_static_html(text.get_body_part());
	tmp_select->give(tmp_compare);
	tmp_compare = NULL;

	    // configuration of the test subpage
	text.clear();
	text.add_text(1, "test page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	tmp_test->give_static_html(text.get_body_part());
	tmp_select->give(tmp_test);
	tmp_test = NULL;

	    // configuration of the list subpage
	tmp_select->give(tmp_list);
	tmp_list = NULL;

	    // configuration of the create subpage
	text.clear();
	text.add_text(1, "create page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	tmp_create->give_static_html(text.get_body_part());
	tmp_select->give(tmp_create);
	tmp_create = NULL;

	    // configuration of the isolate subpage
	text.clear();
	text.add_text(1, "isolation page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	tmp_isolate->give_static_html(text.get_body_part());
	tmp_select->give(tmp_isolate);
	tmp_isolate = NULL;

	    // configuration of the merge subpage
	text.clear();
	text.add_text(1, "merge page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	tmp_merge->give_static_html(text.get_body_part());
	tmp_select->give(tmp_merge);
	tmp_merge = NULL;

	    // configuration of the filters subpage
	text.clear();
	text.add_text(1, "filters page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	tmp_filters->give_static_html(text.get_body_part());
	tmp_select->give(tmp_filters);
	tmp_filters = NULL;

	    // configuration of the sep subpage
	text.clear();
	text.add_text(1, "BUG: THIS separation PAGE SHOULD NEVER SHOW");
	tmp_sep->give_static_html(text.get_body_part());
	tmp_select->give(tmp_sep);
	tmp_sep = NULL;

	    // configuration of the close subpage
	text.clear();
	text.add_text(1, "session close page");
	text.add_nl();
	text.add_text(0, "to be implemented");
	tmp_close->give_static_html(text.get_body_part());
	tmp_select->give(tmp_close);
	tmp_close = NULL;

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
	if(tmp_about != NULL)
	    delete tmp_about;
	if(tmp_restore != NULL)
	    delete tmp_restore;
	if(tmp_compare != NULL)
	    delete tmp_compare;
	if(tmp_test != NULL)
	    delete tmp_test;
	if(tmp_list != NULL)
	    delete tmp_list;
	if(tmp_create != NULL)
	    delete tmp_create;
	if(tmp_isolate != NULL)
	    delete tmp_isolate;
	if(tmp_merge != NULL)
	    delete tmp_merge;
	if(tmp_filters != NULL)
	    delete tmp_filters;
	if(tmp_sep != NULL)
	    delete tmp_sep;
	if(tmp_close != NULL)
	    delete tmp_close;
	if(tmp_sessions != NULL)
	    delete tmp_sessions;
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

    (void)choice->get_body_part(sub_path, req);
    set_title(string("WEBDAR - ") + choice->get_current_label());
    select->set_mode(choice->get_current_mode());

    return html_page::get_body_part(path, req);
}


