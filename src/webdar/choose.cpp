    // C system header files
extern "C"
{

}

    // C++ system header files
#include <new>

    // webdar headers
#include "html_form.hpp"
#include "html_table.hpp"
#include "html_page.hpp"
#include "html_url.hpp"
#include "html_text.hpp"
#include "html_form_fieldset.hpp"
#include "tokens.hpp"
#include "html_div.hpp"
    //
#include "choose.hpp"

using namespace std;

map<string, choose::record *> choose::per_user;

choose::choose(const std::string & user)
{
    owner = user;
    confirm_mode = false;
    page = NULL;
    confirm = NULL;
    confirmed = NULL;

    boxes.clear();

    try
    {
	page = new (nothrow) html_page("Choose a session");
	confirm = new (nothrow) html_page("Killing a session");

	if(page == NULL || confirm == NULL)
	    throw exception_memory();

	page->css_background_color(COLOR_BACK);
	page->css_color(COLOR_TEXT);
	page->css_padding("1em");
	page->css_text_align(css::al_center);

	confirm->css_background_color(COLOR_BACK);
	confirm->css_color(COLOR_TEXT, true);
	confirm->css_text_align(css::al_center);
    }
    catch(...)
    {
	if(page != NULL)
	    delete page;
	if(confirm != NULL)
	    delete confirm;
	throw;
    }
}

choose::~choose()
{
    if(page != NULL)
	delete page;
    if(confirm != NULL)
	delete confirm;
};


answer choose::give_answer(const request & req)
{
    answer ret;

	// update the form fields when request is a POST

    if(req.get_method() == "POST")
    {
	if(confirm_mode)
	{
	    if(confirm == NULL)
		throw WEBDAR_BUG;

		// updating form fields
	    (void)confirm->get_body_part(req.get_uri().get_path(), req);

	    if(confirmed == NULL)
		throw WEBDAR_BUG;
	    if(confirmed->get_selected_num() == 1) // kill confirmed
		kill_selected_sessions();
	    confirm_mode = false;
	}
	else
	{
	    vector<html_form_input *>::iterator itb = boxes.begin();

	    if(page == NULL)
		throw WEBDAR_BUG;

		// updating form fields
	    (void)page->get_body_part(req.get_uri().get_path(), req);

	    while(itb != boxes.end() && (*itb) != NULL && (*itb)->get_value() == "")
		++itb;

	    if(itb != boxes.end() && (*itb) == NULL)
		throw WEBDAR_BUG;

	    if(itb != boxes.end()) // at least one box has been checked
		confirm_mode = true;
	}
    }

	// generate response HTML page

    ret.set_status(STATUS_CODE_OK);
    ret.set_reason("ok");

    if(confirm_mode) // confirmation requested for session kill
    {
	regenerate_confirm_page();

	if(confirm == NULL)
	    throw WEBDAR_BUG;
	ret.add_body(confirm->get_body_part(req.get_uri().get_path(), req));
    }
    else // normal table display
    {

	regenerate_table_page();

	if(req.get_uri().get_path() == chemin("choose/new"))
	    ret = create_new_session(req);
	else
	{
	    if(page == NULL)
		throw WEBDAR_BUG;
	    ret.add_body(page->get_body_part(req.get_uri().get_path(), req));
	}
    }

    return ret;
}

answer choose::create_new_session(const request & req)
{
    answer ret;

    html_page page = html_page("redirection to newly created session page");
    string session_ID = session::create_new(owner);

    page.set_refresh_redirection(0, session_ID);
    ret.set_status(STATUS_CODE_OK);
    ret.set_reason("new session created");
    ret.add_body(page.get_body_part(chemin("/"), req));

    return ret;
}

void choose::regenerate_table_page()
{
    html_table *table = NULL;
    html_url *nouvelle = NULL;
    html_form *form = NULL;
    html_form_input *check = NULL;
    html_div *div = NULL;

	// releasing old objects and list

    if(page == NULL)
	throw WEBDAR_BUG;
    boxes.clear();
    page->clear();

	// creating a new page

    table = new (nothrow) html_table(5);
    nouvelle = new (nothrow) html_url("/choose/new", "Create a new session");
    form = new (nothrow) html_form("Kill the selected session");
    div = new (nothrow) html_div();
    sess = session::get_summary();
    try
    {
	html_text tmp;
	unsigned int count = 0;

	if(table == NULL
	   || nouvelle == NULL
	   || form == NULL
	   || div == NULL)
	    throw exception_memory();

	tmp.add_text(3, string("Current sessions for user ") + owner);
	tmp.css_padding("1em");
	tmp.css_background_color(COLOR_PADBACK);
	tmp.css_color(COLOR_PADFRONT);
	tmp.css_font_weight_bold();
	tmp.css_border_width(css::bd_all, css::bd_medium);
	tmp.css_border_style(css::bd_all, css::bd_solid);
	tmp.css_border_color(css::bd_all, COLOR_PADBORD);
	page->give_static_html(tmp.get_body_part());

	table->set_border(1);
	table->give_static_html("Session ID");
	table->give_static_html("Locked");
	table->give_static_html("Running");
	table->give_static_html("Closing");
	table->give_static_html("Kill Session");

	for(vector<session::session_summary>::iterator it = sess.begin();
	    it != sess.end();
	    ++it)
	{
	    if(it->owner == owner)
	    {
		++count;
		table->give_static_html(html_url(string("/") + it->session_ID, string("Session ") + webdar_tools_convert_to_string(count)).get_body_part());
		table->give_static_html(it->locked ? "locked" : "");
		table->give_static_html(it->libdar_running ? "running" : "");
		table->give_static_html(it->closing ? "closing" : "");
		check = new (nothrow) html_form_input("", html_form_input::check, "", 10);
		if(check == NULL)
		    throw exception_memory();
		boxes.push_back(check);
		table->give(check);
		check = NULL;
	    }
	}

	div->css_width("90%", true);
	div->give(table);
	table = NULL;
	form->give(div);
	div = NULL;
	page->give(form);
	form = NULL;
	page->give(nouvelle);
	nouvelle = NULL;
	page->set_prefix(chemin("choose"));
    }
    catch(...)
    {
	if(table != NULL)
	    delete table;
	if(nouvelle != NULL)
	    delete nouvelle;
	if(form != NULL)
	    delete form;
	if(check != NULL)
	    delete check;
	if(div != NULL)
	    delete div;
	boxes.clear();
	throw;
    }
}

void choose::regenerate_confirm_page()
{
    html_table *table = NULL;
    html_form *form = NULL;
    html_form_fieldset *block = NULL;
    html_form_radio * radio = NULL;

    if(confirm == NULL)
	throw WEBDAR_BUG;

    confirm->clear();
    confirmed = NULL;

    try
    {
	html_text text;

	table = new (nothrow) html_table(1);
	form = new (nothrow) html_form("Submit");
	block = new (nothrow) html_form_fieldset("Confirm destruction of sessions listed above?");
	radio = new (nothrow) html_form_radio();
	if(table == NULL || form == NULL || block == NULL || radio == NULL)
	    throw exception_memory();

	text.add_text(3, "The following sessions are about to be destroyed");
	text.css_padding("1em");
	text.css_background_color("red");
	text.css_color(COLOR_PADFRONT);
	text.css_font_weight_bold();
	text.css_border_width(css::bd_all, css::bd_medium);
	text.css_border_style(css::bd_all, css::bd_solid);
	text.css_border_color(css::bd_all, COLOR_PADBORD);
	table->give_static_html(text.get_body_part());

	text.css_clear_attributes();
	text.css_padding_left("1em");

	if(boxes.size() != sess.size())
	    throw WEBDAR_BUG;
	for(unsigned int i = 0; i < boxes.size(); ++i)
	{
	    if(boxes[i] == NULL)
		throw WEBDAR_BUG;
	    if(boxes[i]->get_value() != "")
	    {
		text.clear();
		text.add_text(0,
			      html_url(chemin(sess[i].session_ID).display(false), string("Session ") + webdar_tools_convert_to_string(i+1)).get_body_part());

		table->give_static_html(text.get_body_part());
	    }
	}

	radio->add_choice("no", "No");
	radio->add_choice("yes", "Yes");
	radio->set_selected(0);

	block->give(radio);
	confirmed = radio;
	radio = NULL;

	form->give(block);
	block = NULL;

	table->give(form);
	form = NULL;

	confirm->give(table);
	table = NULL;
	confirm->set_prefix(chemin("choose"));
    }
    catch(...)
    {
	if(table != NULL)
	    delete table;
	if(form != NULL)
	    delete form;
	if(block != NULL)
	    delete block;
	if(radio != NULL)
	    delete radio;
	confirmed = NULL;
	throw;
    }
}

void choose::kill_selected_sessions() const
{
    if(page == NULL)
	throw WEBDAR_BUG;
    if(boxes.size() != sess.size())
	throw WEBDAR_BUG;

    for(unsigned int i = 0;
	i < boxes.size();
	++i)
    {
	if(boxes[i] == NULL)
	    throw WEBDAR_BUG;
	if(boxes[i]->get_value() != "") // session to be killed
	    (void)session::close_session(sess[i].session_ID);
    }
}


answer choose::give_answer_for(const std::string & user, const request & req)
{
    answer ret;

    map<string, record *>::iterator it = per_user.find(user);

    if(it == per_user.end())
    {
	record *tmp = new (nothrow) record();
	if(tmp == NULL)
	    throw exception_memory();

	try
	{
	    tmp->obj = new (nothrow) choose(user);
	    if(tmp->obj == NULL)
		throw exception_memory();
	    ret = tmp->obj->create_new_session(req);

	    per_user[user] = tmp;
	}
	catch(...)
	{
	    delete tmp;
	    throw;
	}

	it = per_user.find(user);
	if(it == per_user.end())
	    throw WEBDAR_BUG;
    }
    else
    {
	if(it->second == NULL)
	    throw WEBDAR_BUG;

	it->second->lock.lock();
	try
	{
	    if(it->second->obj == NULL)
		throw WEBDAR_BUG;

	    ret = it->second->obj->give_answer(req);
	}
	catch(...)
	{
	    it->second->lock.unlock();
	    throw;
	}

	it->second->lock.unlock();
    }

    return ret;
}


void choose::cleanup_memory()
{
    map<string, record *>::iterator it = per_user.begin();

    try
    {

	while(it != per_user.end())
	{
	    if(it->second != NULL)
		delete it->second;
	    ++it;
	}
    }
    catch(...)
    {
	per_user.clear();
	throw;
    }

    per_user.clear();
}

