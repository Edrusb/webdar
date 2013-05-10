    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "web_user_interaction.hpp"

web_user_interaction_html_data::web_user_interaction_html_data():
    get_string("", html_form_input::text, "", 20),
    inter("User interaction requested from libdar"),
    form("Update"),
    logs("Last logs"),
    global("Interaction with libdar")
{
    pause2.add_choice("undefined", "please answer yes or no");
    pause2.add_choice("no", "No");
    pause2.add_choice("yes", "Yes");

    inter.adopt(&pause2);
    form.adopt(&inter);
    form.adopt(&get_string);
    global.adopt(&form);
    logs.adopt(&warnings);
    global.adopt(&logs);

    inter.set_visible(false);
    get_string.set_visible(false);
};


using namespace std;

web_user_interaction::web_user_interaction(unsigned int x_warn_size)
{
    lib_data = NULL;
    html_data = NULL;
    try
    {
	lib_data = new (nothrow) web_user_interaction_libdar_data();
	html_data = new (nothrow) web_user_interaction_html_data();

	if(lib_data == NULL)
	    throw exception_memory();
	if(html_data == NULL)
	    throw exception_memory();

	lib_data->instances = 1;
	lib_data->pause2_pending = false;
	lib_data->get_string_pending = false;
	lib_data->get_secu_string_pending = false;
	lib_data->warn_size = x_warn_size;
	    // acquiring the libdar pending semaphore for libdar thread get frozen waiting of the html thread to obtain an answer for the user
	lib_data->libdar_sem.lock();

	adopt(&html_data->global);

	html_data->pause2.record_actor_on_event(this, html_form_radio::changed);
	html_data->get_string.record_actor_on_event(this, html_form_input::changed);
    }
    catch(...)
    {
	if(lib_data != NULL)
	    delete lib_data;
	if(html_data != NULL)
	    delete html_data;
	throw;
    }
}

void web_user_interaction::set_warning_list_size(unsigned int size)
{
    if(lib_data == NULL)
	throw WEBDAR_BUG;

	// critical section begins
    lib_data->control.lock();
    try
    {
	lib_data->warn_size = size;
	while(lib_data->warnings.size() > size)
	    lib_data->warnings.pop_front();
    }
    catch(...)
    {
	lib_data->control.unlock();
	throw;
    }
    lib_data->control.unlock();
	// critical section ended
}

bool web_user_interaction::pause2(const std::string & message)
{
    bool ret;

    if(lib_data == NULL)
	throw WEBDAR_BUG;

	// critical section begins
    lib_data->control.lock();
    try
    {
	if(lib_data->pause2_pending || lib_data->get_string_pending || lib_data->get_secu_string_pending)
	    throw WEBDAR_BUG; // already waiting for an answer!
	lib_data->answered = false;
	lib_data->pause2_msg = message;
	lib_data->pause2_pending = true;
    }
    catch(...)
    {
	lib_data->control.unlock();
	throw;
    }
    lib_data->control.unlock();
	// critical section ended

    lib_data->libdar_sem.lock();

	// critical section begins
    lib_data->control.lock();
    try
    {
	if(!lib_data->pause2_pending)
	    throw WEBDAR_BUG; // answer already read!
	lib_data->pause2_pending = false;
	ret = lib_data->pause2_ans;
	lib_data->pause2_msg = "";
    }
    catch(...)
    {
	lib_data->control.unlock();
	throw;
    }
    lib_data->control.unlock();
	// critical section ended

    return ret;
}

string web_user_interaction::get_string(const std::string & message, bool echo)
{
    string ret;

    if(lib_data == NULL)
	throw WEBDAR_BUG;

	// critical section begins
    lib_data->control.lock();
    try
    {
	if(lib_data->pause2_pending || lib_data->get_string_pending || lib_data->get_secu_string_pending)
	    throw WEBDAR_BUG; // already waiting for an answer!
	lib_data->answered = false;
	lib_data->get_string_msg = message;
	lib_data->get_string_echo = echo;
	lib_data->get_string_pending = true;
    }
    catch(...)
    {
	lib_data->control.unlock();
	throw;
    }
    lib_data->control.unlock();
	// critical section ended

    lib_data->libdar_sem.lock();

	// critical section begins
    lib_data->control.lock();
    try
    {
	if(!lib_data->get_string_pending)
	    throw WEBDAR_BUG; // answer already read!
	lib_data->get_string_pending = false;
	ret = lib_data->get_string_ans;
	lib_data->get_string_msg = "";
	lib_data->get_string_ans = "";
    }
    catch(...)
    {
	lib_data->control.unlock();
	throw;
    }
    lib_data->control.unlock();
	// critical section ended

    return ret;
}

libdar::secu_string web_user_interaction::get_secu_string(const std::string & message, bool echo)
{
    libdar::secu_string ret;

    if(lib_data == NULL)
	throw WEBDAR_BUG;

	// critical section begins
    lib_data->control.lock();
    try
    {
	if(lib_data->pause2_pending || lib_data->get_string_pending || lib_data->get_secu_string_pending)
	    throw WEBDAR_BUG; // already waiting for an answer!
	lib_data->answered = false;
	lib_data->get_secu_string_msg = message;
	lib_data->get_secu_string_echo = echo;
	lib_data->get_secu_string_pending = true;
    }
    catch(...)
    {
	lib_data->control.unlock();
	throw;
    }
    lib_data->control.unlock();
	// critical section ended

    lib_data->libdar_sem.lock();

	// critical section begins
    lib_data->control.lock();
    try
    {
	if(!lib_data->get_secu_string_pending)
	    throw WEBDAR_BUG; // answer already read!
	lib_data->get_secu_string_pending = false;
	ret = lib_data->get_secu_string_ans;
	lib_data->get_string_msg = "";
	lib_data->get_string_ans = "";
    }
    catch(...)
    {
	lib_data->control.unlock();
	throw;
    }
    lib_data->control.unlock();
	// critical section ended

    return ret;
}


libdar::user_interaction *web_user_interaction::clone() const
{
    libdar::user_interaction *ret = new (nothrow) web_user_interaction(*this);
    if(ret == NULL)
	throw libdar::Ememory("web_user_interaction");

    return ret;
}


string web_user_interaction::get_body_part(const chemin & path,
					   const request & req)
{
    string ret;
    bool clear_pause2 = false;
    bool clear_get_string = false;

    if(lib_data == NULL)
	throw WEBDAR_BUG;
    if(html_data == NULL)
	throw WEBDAR_BUG;

	// critical section begins
    lib_data->control.lock();
    try
    {
	html_data->warnings.clear();
	for(list<string>::iterator it = lib_data->warnings.begin();
	    it != lib_data->warnings.end();
	    ++it)
	    html_data->warnings.add_text(0, *it);

	if(lib_data->pause2_pending && !lib_data->answered)
	{
	    if(!html_data->pause2.get_visible());
	    {
		html_data->pause2.set_visible(true);
		html_data->inter.change_label(lib_data->pause2_msg);
		clear_pause2 = true;
	    }
	}

	if(lib_data->get_string_pending && !lib_data->answered)
	{
	    if(!html_data->get_string.get_visible())
	    {
		html_data->get_string.set_visible(true);
		html_data->get_string.change_label(lib_data->get_string_msg);
		if(lib_data->get_string_echo)
		    html_data->get_string.change_type(html_form_input::text);
		else
		    html_data->get_string.change_type(html_form_input::password);
		clear_get_string = true;
	    }
	}

	if(lib_data->get_secu_string_pending && !lib_data->answered)
	{
	    if(!html_data->get_string.get_visible())
	    {
		html_data->get_string.set_visible(true);
		html_data->get_string.change_label(lib_data->get_secu_string_msg);
		if(lib_data->get_string_echo)
		    html_data->get_string.change_type(html_form_input::text);
		else
		    html_data->get_string.change_type(html_form_input::password);
		clear_get_string = true;
	    }
	}
    }
    catch(...)
    {
	lib_data->control.unlock();
	throw;
    }
    lib_data->control.unlock();
	// critical section ended

	// these two actions must be done out of critical section because
	// it triggers on_event() that need to access the critical section too
    if(clear_pause2)
	html_data->pause2.set_selected(0);
    if(clear_get_string)
	html_data->get_string.set_value("");

    html_data->rebuild_body_part = false;
    ret = get_body_part_from_all_children(path, req);
    if(html_data->rebuild_body_part)
	ret = get_body_part_from_all_children(path, req);

    return ret;
}

void web_user_interaction::on_event(const std::string & event_name)
{
    if(lib_data == NULL)
	throw WEBDAR_BUG;
    if(html_data == NULL)
	throw WEBDAR_BUG;

	// critical section begins
    lib_data->control.lock();
    try
    {

	if(html_data->inter.get_visible())
	{
	    if(lib_data->pause2_pending)
	    {
		lib_data->pause2_ans = (html_data->pause2.get_selected_num() == 2);
		lib_data->answered = true;
		lib_data->libdar_sem.unlock();
		html_data->inter.set_visible(false);
		html_data->rebuild_body_part = true;
	    }
	    else
		throw WEBDAR_BUG; // inter is visible while no pause2() has been asked
	}

	if(html_data->get_string.get_visible())
	{
	    if(lib_data->get_string_pending)
	    {
		lib_data->get_string_ans = html_data->get_string.get_value();
		lib_data->answered = true;
		lib_data->libdar_sem.unlock();
		html_data->get_string.set_visible(false);
		html_data->rebuild_body_part = true;
	    }
	    else
	    {
		if(lib_data->get_secu_string_pending)
		{
		    lib_data->get_secu_string_ans = libdar::secu_string(html_data->get_string.get_value().c_str(), html_data->get_string.get_value().size());
		    lib_data->answered = true;
		    lib_data->libdar_sem.unlock();
		    html_data->get_string.set_visible(false);
		    html_data->rebuild_body_part = true;
		}
		else
		    throw WEBDAR_BUG;
	    }
	}
    }
    catch(...)
    {
	lib_data->control.unlock();
	throw;
    }
    lib_data->control.unlock();
	// critical section ended
}

void web_user_interaction::inherited_warning(const std::string & message)
{
    if(lib_data == NULL)
	throw WEBDAR_BUG;

    	// critical section begins
    lib_data->control.lock();
    try
    {
	lib_data->warnings.push_back(message);
	if(lib_data->warnings.size() >= lib_data->warn_size)
	    lib_data->warnings.pop_front();
    }
    catch(...)
    {
	lib_data->control.unlock();
	throw;
    }
    lib_data->control.unlock();
	// critical section ended
}

void web_user_interaction::copy_from(const web_user_interaction & ref)
{
    bool error = false;

    if(ref.lib_data == NULL)
	throw WEBDAR_BUG;
    if(ref.html_data == NULL)
	throw WEBDAR_BUG;

    lib_data = const_cast<web_user_interaction_libdar_data *>(ref.lib_data);
    if(lib_data == NULL)
	throw WEBDAR_BUG;
    html_data = const_cast<web_user_interaction_html_data *>(ref.html_data);
    if(html_data == NULL)
	throw WEBDAR_BUG;

	// critical section begins
    lib_data->control.lock();
    try
    {
	++(lib_data->instances);
	if(lib_data->instances < 2)
	    error = true;
    }
    catch(...)
    {
	lib_data->control.unlock();
	throw;
    }
    lib_data->control.unlock();
	// critical section ended

    if(error)
	throw WEBDAR_BUG;
}

void web_user_interaction::destroy()
{
    bool destruction = false;

    if(lib_data == NULL)
	throw WEBDAR_BUG;
    if(html_data == NULL)
	throw WEBDAR_BUG;

	// critical section begins
    lib_data->control.lock();
    try
    {
	--(lib_data->instances);
	if(lib_data->instances == 0)
	{
	    destruction = true;
	    if(lib_data->pause2_pending || lib_data->get_string_pending || lib_data->get_secu_string_pending)
		throw WEBDAR_BUG;
	}
    }
    catch(...)
    {
	lib_data->control.unlock();
	throw;
    }
    lib_data->control.unlock();
	// critical section ended

    if(destruction)
    {
	delete lib_data;
	delete html_data;
    }
    lib_data = NULL; // yes, in any case
    html_data = NULL; // yes, in any case
}

