    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "web_user_interaction.hpp"


using namespace std;

web_user_interaction::web_user_interaction(unsigned int x_warn_size):
    h_inter("User interaction requested from libdar"),
    h_get_string("", html_form_input::text, "", 20),
    h_form("Update"),
    h_logs("Last logs"),
    h_global("Interaction with libdar"),
    lib_data(x_warn_size)
{
    h_pause2.add_choice("undefined", "please answer yes or no");
    h_pause2.add_choice("no", "No");
    h_pause2.add_choice("yes", "Yes");

    h_inter.adopt(&h_pause2);
    h_form.adopt(&h_inter);
    h_form.adopt(&h_get_string);
    h_global.adopt(&h_form);
    h_logs.adopt(&h_warnings);
    h_global.adopt(&h_logs);
    adopt(&h_global);

    h_inter.set_visible(false);
    h_get_string.set_visible(false);
    h_form.set_visible(false);

    h_pause2.record_actor_on_event(this, html_form_radio::changed);
    h_get_string.record_actor_on_event(this, html_form_input::changed);
}

web_user_interaction::web_user_interaction(const web_user_interaction & ref):
    h_inter(""),
    h_get_string("", html_form_input::text, "", 1),
    h_form(""),
    h_logs(""),
    h_global(""),
    lib_data(1)
{
    throw WEBDAR_BUG;
}

void web_user_interaction::set_warning_list_size(unsigned int size)
{

	// critical section begins
	//
    lib_data.control.lock();
    try
    {
	lib_data.warn_size = size;
	while(lib_data.warnings.size() > size)
	    lib_data.warnings.pop_front();
    }
    catch(...)
    {
	lib_data.control.unlock();
	throw;
    }
    lib_data.control.unlock();
	//
	// critical section ended
}

bool web_user_interaction::pause2(const std::string & message)
{
    bool ret;

	// critical section begins
	//
    lib_data.control.lock();
    try
    {
	if(lib_data.pause2_pending || lib_data.get_string_pending || lib_data.get_secu_string_pending)
	    throw WEBDAR_BUG; // already waiting for an answer!
	lib_data.answered = false;
	lib_data.pause2_msg = message;
	lib_data.pause2_pending = true;
    }
    catch(...)
    {
	lib_data.control.unlock();
	throw;
    }
    lib_data.control.unlock();
	//
	// critical section ended

	// waiting on this semaphor for the answer to be filled by the html thread
    lib_data.libdar_sem.lock();
	// the answer has been filled so we use it to return the info to libdar

	// critical section begins
	//
    lib_data.control.lock();
    try
    {
	if(!lib_data.pause2_pending)
	    throw WEBDAR_BUG; // answer already read!?!
	lib_data.pause2_pending = false;
	ret = lib_data.pause2_ans;
	lib_data.pause2_msg = "";
    }
    catch(...)
    {
	lib_data.control.unlock();
	throw;
    }
    lib_data.control.unlock();
	//
	// critical section ended

    return ret;
}

string web_user_interaction::get_string(const std::string & message, bool echo)
{
    string ret;

	// critical section begins
	//
    lib_data.control.lock();
    try
    {
	if(lib_data.pause2_pending || lib_data.get_string_pending || lib_data.get_secu_string_pending)
	    throw WEBDAR_BUG; // already waiting for an answer!
	lib_data.answered = false;
	lib_data.get_string_msg = message;
	lib_data.get_string_echo = echo;
	lib_data.get_string_pending = true;
    }
    catch(...)
    {
	lib_data.control.unlock();
	throw;
    }
    lib_data.control.unlock();
	//
	// critical section ended

	// waiting on this semaphor for the answer to be filled by the html thread
    lib_data.libdar_sem.lock();
	// the answer has been filled so we use it to return the info to libdar

	// critical section begins
	//
    lib_data.control.lock();
    try
    {
	if(!lib_data.get_string_pending)
	    throw WEBDAR_BUG; // answer already read!
	lib_data.get_string_pending = false;
	ret = lib_data.get_string_ans;
	lib_data.get_string_msg = "";
	lib_data.get_string_ans = "";
    }
    catch(...)
    {
	lib_data.control.unlock();
	throw;
    }
    lib_data.control.unlock();
	//
	// critical section ended

    return ret;
}

libdar::secu_string web_user_interaction::get_secu_string(const std::string & message, bool echo)
{
    libdar::secu_string ret;

	// critical section begins
	//
    lib_data.control.lock();
    try
    {
	if(lib_data.pause2_pending || lib_data.get_string_pending || lib_data.get_secu_string_pending)
	    throw WEBDAR_BUG; // already waiting for an answer!
	lib_data.answered = false;
	lib_data.get_secu_string_msg = message;
	lib_data.get_secu_string_echo = echo;
	lib_data.get_secu_string_pending = true;
    }
    catch(...)
    {
	lib_data.control.unlock();
	throw;
    }
    lib_data.control.unlock();
	//
	// critical section ended

	// waiting on this semaphor for the answer to be filled by the html thread
    lib_data.libdar_sem.lock();
	// the answer has been filled so we use it to return the info to libdar

	// critical section begins
	//
    lib_data.control.lock();
    try
    {
	if(!lib_data.get_secu_string_pending)
	    throw WEBDAR_BUG; // answer already read!
	lib_data.get_secu_string_pending = false;
	ret = lib_data.get_secu_string_ans;
	lib_data.get_string_msg = "";
	lib_data.get_string_ans = "";
    }
    catch(...)
    {
	lib_data.control.unlock();
	throw;
    }
    lib_data.control.unlock();
	//
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

	// in the following we need to ignore on_event() calls to be able to
	// se the html_interface without considering it as
	// an interaction from the user
    ignore_event = true;

    try
    {
	    // critical section begins
	    //
	lib_data.control.lock();
	try
	{
	    h_warnings.clear();
	    for(list<string>::iterator it = lib_data.warnings.begin();
		it != lib_data.warnings.end();
		++it)
	    {
		h_warnings.add_text(0, *it);
		h_warnings.add_nl();
	    }

	    if(lib_data.pause2_pending && !lib_data.answered)
	    {
		if(!h_inter.get_visible());
		{
		    h_inter.set_visible(true);
		    h_inter.change_label(lib_data.pause2_msg);
		    h_pause2.set_selected(0);
		}
	    }

	    if(lib_data.get_string_pending && !lib_data.answered)
	    {
		if(!h_get_string.get_visible())
		{
		    h_get_string.set_visible(true);
		    h_get_string.change_label(lib_data.get_string_msg);
		    if(lib_data.get_string_echo)
			h_get_string.change_type(html_form_input::text);
		    else
			h_get_string.change_type(html_form_input::password);
		    h_get_string.set_value("");
		}
	    }

	    if(lib_data.get_secu_string_pending && !lib_data.answered)
	    {
		if(!h_get_string.get_visible())
		{
		    h_get_string.set_visible(true);
		    h_get_string.change_label(lib_data.get_secu_string_msg);
		    if(lib_data.get_string_echo)
			h_get_string.change_type(html_form_input::text);
		    else
			h_get_string.change_type(html_form_input::password);
		    h_get_string.set_value("");
		}
	    }

	    if(h_get_string.get_visible() || h_inter.get_visible())
		h_form.set_visible(true);
	    else
		h_form.set_visible(false);
	}
	catch(...)
	{
	    lib_data.control.unlock();
	    throw;
	}
	lib_data.control.unlock();
	    //
	    // critical section ended
    }
    catch(...)
    {
	ignore_event = false;
	throw;
    }
    ignore_event = false;

	// now we return to the user the updated html interface
	// any event is a user event and may need further display

    rebuild_body_part = false;
    ret = get_body_part_from_all_children(path, req);
    if(rebuild_body_part)
	ret = get_body_part_from_all_children(path, req);

    return ret;
}

void web_user_interaction::on_event(const std::string & event_name)
{
    if(ignore_event)
	return;

	// critical section begins
	//
    lib_data.control.lock();
    try
    {
	if(h_inter.get_visible())
	{
	    if(lib_data.pause2_pending)
	    {
		lib_data.pause2_ans = (h_pause2.get_selected_num() == 2);
		lib_data.answered = true;
		lib_data.libdar_sem.unlock(); // releasing libdar thread
		h_inter.set_visible(false);
		rebuild_body_part = true;
	    }
	    else
		throw WEBDAR_BUG; // inter is visible while no pause2() has been asked
	}

	if(h_get_string.get_visible())
	{
	    if(lib_data.get_string_pending)
	    {
		lib_data.get_string_ans = h_get_string.get_value();
		lib_data.answered = true;
		lib_data.libdar_sem.unlock(); // releasing libdar thread
		h_get_string.set_visible(false);
		rebuild_body_part = true;
	    }
	    else
	    {
		if(lib_data.get_secu_string_pending)
		{
		    lib_data.get_secu_string_ans = libdar::secu_string(h_get_string.get_value().c_str(), h_get_string.get_value().size());
		    lib_data.answered = true;
		    lib_data.libdar_sem.unlock(); // releasing libdar thread
		    h_get_string.set_visible(false);
		    rebuild_body_part = true;
		}
		else
		    throw WEBDAR_BUG;
	    }
	}
    }
    catch(...)
    {
	lib_data.control.unlock();
	throw;
    }
    lib_data.control.unlock();
	//
	// critical section ended
}

void web_user_interaction::clear()
{
    lib_data.control.lock();
    try
    {
	lib_data.warnings.clear();
    }
    catch(...)
    {
	lib_data.control.unlock();
	throw;
    }
    lib_data.control.unlock();
}

bool web_user_interaction::can_refresh() const
{
    bool ret = false;
    web_user_interaction *me = const_cast<web_user_interaction *>(this);

    if(me == NULL)
	throw WEBDAR_BUG;
    me->lib_data.control.lock();

    try
    {
	ret = !lib_data.pause2_pending
	    && !lib_data.get_string_pending
	    && !lib_data.get_secu_string_pending;
    }
    catch(...)
    {
	me->lib_data.control.unlock();
	throw;
    }
    me->lib_data.control.unlock();


    return ret;
}

void web_user_interaction::inherited_warning(const std::string & message)
{
    	// critical section begins
	//
    lib_data.control.lock();
    try
    {
	lib_data.warnings.push_back(message);
	while(lib_data.warnings.size() >= lib_data.warn_size)
	    lib_data.warnings.pop_front();
    }
    catch(...)
    {
	lib_data.control.unlock();
	throw;
    }
    lib_data.control.unlock();
	//
	// critical section ended
}

void web_user_interaction::destroy()
{
	// critical section begins
	//
    lib_data.control.lock();
    try
    {
	--(lib_data.instances);
	if(lib_data.instances == 0)
	{
	    if(lib_data.pause2_pending || lib_data.get_string_pending || lib_data.get_secu_string_pending)
		throw WEBDAR_BUG; // libdar is pending for an answer!
	}
    }
    catch(...)
    {
	lib_data.control.unlock();
	throw;
    }
    lib_data.control.unlock();
	//
	// critical section ended
}


///////////// web_user_interaction_libdar_data methods //////////////////

web_user_interaction_libdar_data::web_user_interaction_libdar_data(unsigned int x_warn_size)
{
    instances = 1;
    pause2_pending = false;
    get_string_pending = false;
    get_secu_string_pending = false;
    warn_size = x_warn_size;
	// acquiring the libdar pending semaphore from libdar thread will freeze the thread waiting for the html thread to obtain an answer for the user
    libdar_sem.lock();
}

web_user_interaction_libdar_data::~web_user_interaction_libdar_data()
{
    if(instances == 0)
	libdar_sem.unlock();
    else
	throw WEBDAR_BUG;
}
