    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "webdar_tools.hpp"
#include "user_interface.hpp"

    //
#include "session.hpp"

using namespace std;

const unsigned int SESSION_ID_WIDTH = 8;


    //////////////////////////
    //  object method implementation
    //

session::session()
{
    libdar_running = false;
    current_thread = NULL;
    session_ID = "";
    tid = 0;

    wui.record_actor_on_event(this, user_interface::closing);
    wui.record_actor_on_event(this, user_interface::ask_end_libdar);
    wui.record_actor_on_event(this, user_interface::force_end_libdar);
    wui.record_actor_on_event(this, user_interface::kill_libdar_thread);
    wui.record_actor_on_event(this, user_interface::clean_ended_libdar);
    wui.record_actor_on_event(this, user_interface::start_restore);
    wui.record_actor_on_event(this, user_interface::start_compare);
    wui.record_actor_on_event(this, user_interface::start_test);
    wui.record_actor_on_event(this, user_interface::start_create);
    wui.record_actor_on_event(this, user_interface::start_isolate);
    wui.record_actor_on_event(this, user_interface::start_merge);
}

void session::set_session_id(const string & sessid)
{
    session_ID = sessid;
    wui.set_prefix(chemin(sessid));
}

void session::check_caller() const
{
    if(pthread_self() != tid)
	throw WEBDAR_BUG;
    if(session_ID == "")
	throw WEBDAR_BUG;
}

answer session::give_answer(const request & req)
{
    check_caller();
    if(libdar_running)
    {
	if(current_thread == NULL)
	    throw WEBDAR_BUG;
	if(!current_thread->is_running())
	    wui.libdar_has_finished();
    }

    return wui.give_answer(req);
}

void session::on_event(const std::string & event_name)
{
    if(event_name == user_interface::closing)
	close_session(get_session_ID()); // will kill libdar thread if running
    else if(event_name == user_interface::ask_end_libdar)
    {
	if(libdar_running)
	{
	    if(current_thread != NULL)
	    {
		pthread_t libdar_tid;
		if(current_thread->is_running(libdar_tid))
		{
		    libdar::thread_cancellation th;
		    th.cancel(libdar_tid, false, 0);
		}
	    }
	    else
		throw WEBDAR_BUG;
	}
    }
    else if(event_name == user_interface::force_end_libdar)
    {
	if(libdar_running)
	{
	    if(current_thread != NULL)
	    {
		pthread_t libdar_tid;
		if(current_thread->is_running(libdar_tid))
		{
		    libdar::thread_cancellation th;
		    th.cancel(libdar_tid, true, 0);
		}
	    }
	    else
		throw WEBDAR_BUG;
	}
    }
    else if(event_name == user_interface::kill_libdar_thread)
    {
	if(libdar_running)
	{
	    if(current_thread != NULL)
	    {
		pthread_t libdar_tid;
		if(current_thread->is_running(libdar_tid))
		{
		    if(current_thread->is_running())
			current_thread->kill();
		}
	    }
	    else
		throw WEBDAR_BUG;
	}
    }
    else if(event_name == user_interface::clean_ended_libdar)
    {
	if(!libdar_running)
	    throw WEBDAR_BUG;
	if(current_thread == NULL)
	    throw WEBDAR_BUG;
	if(current_thread->is_running())
	    throw WEBDAR_BUG;

	libdar_running = false;
	try
	{
	    current_thread->join(); // may throw re-exception that were generated in this dead thread
	}
	catch(exception_base & e)
	{
	    current_thread = NULL;
	    e.change_message(string("Error reported from libdar: ") + e.get_message());
	    throw;
	}
	catch(...)
	{
	    current_thread = NULL;
	    throw;
	}
	current_thread = NULL;
    }
    else if(event_name == user_interface::start_restore)
    {
	if(libdar_running)
	    throw WEBDAR_BUG;
	if(current_thread != NULL)
	    throw WEBDAR_BUG;

	    // providing libdar::parameters
	arch_rest.set_user_interaction(wui.get_user_interaction());
	arch_rest.set_archive_path(wui.get_parametrage().get_archive_path());
	arch_rest.set_archive_basename(wui.get_parametrage().get_archive_basename());
	arch_rest.set_archive_options_read(wui.get_parametrage().get_read_options());
	arch_rest.set_fs_root(wui.get_parametrage().get_fs_root());
	arch_rest.set_archive_options_restore(wui.get_parametrage().get_extraction_options());
	arch_rest.set_progressive_report(wui.get_statistics().get_libdar_statistics());


	    // restting counters and logs
	wui.get_user_interaction().clear();
	wui.get_statistics().clear_counters();
	wui.get_statistics().clear_labels();
	wui.get_statistics().set_treated_label("item(s) restored");
	wui.get_statistics().set_skipped_label("item(s) not restored (not saved in archive)");
	wui.get_statistics().set_tooold_label("item(s) not restored (overwriting policy decision)");
	wui.get_statistics().set_errored_label("item(s) failed to restore (filesystem error)");
	wui.get_statistics().set_ignored_label("item(s) ignored (excluded by filters)");
	wui.get_statistics().set_hard_links_label("hard link(s) restored");
	wui.get_statistics().set_ea_treated_label("inode(s) having their EA restored");
	wui.get_statistics().set_total_label("inode(s) considered");

	    // launching libdar in a separated thread
	arch_rest.run();
	current_thread = & arch_rest;
	libdar_running = true;
    }
    else if(event_name == user_interface::start_compare)
    {
	if(libdar_running)
	    throw WEBDAR_BUG;
	if(current_thread != NULL)
	    throw WEBDAR_BUG;

	    // providing libdar::parameters
	arch_diff.set_user_interaction(wui.get_user_interaction());
	arch_diff.set_archive_path(wui.get_parametrage().get_archive_path());
	arch_diff.set_archive_basename(wui.get_parametrage().get_archive_basename());
	arch_diff.set_archive_options_read(wui.get_parametrage().get_read_options());
	arch_diff.set_fs_root(wui.get_parametrage().get_fs_root());
	arch_diff.set_archive_options_compare(wui.get_parametrage().get_comparison_options());
	arch_diff.set_progressive_report(wui.get_statistics().get_libdar_statistics());


	    // restting counters and logs
	wui.get_user_interaction().clear();
	wui.get_statistics().clear_counters();
	wui.get_statistics().clear_labels();
	wui.get_statistics().set_treated_label("item(s) identical");
	wui.get_statistics().set_errored_label("item(s) do not match those on filesystem");
	wui.get_statistics().set_ignored_label("item(s) ignored (excluded by filters)");
	wui.get_statistics().set_total_label("inode(s) considered");

	    // launching libdar in a separated thread
	arch_diff.run();
	current_thread = & arch_diff;
	libdar_running = true;
    }
    else if(event_name == user_interface::start_test)
    {
	if(libdar_running)
	    throw WEBDAR_BUG;
	if(current_thread != NULL)
	    throw WEBDAR_BUG;

	    // providing libdar::parameters
	arch_test.set_user_interaction(wui.get_user_interaction());
	arch_test.set_archive_path(wui.get_parametrage().get_archive_path());
	arch_test.set_archive_basename(wui.get_parametrage().get_archive_basename());
	arch_test.set_archive_options_read(wui.get_parametrage().get_read_options());
	arch_test.set_archive_options_test(wui.get_parametrage().get_testing_options());
	arch_test.set_progressive_report(wui.get_statistics().get_libdar_statistics());

	    // resetting counters and logs
	wui.get_user_interaction().clear();
	wui.get_statistics().clear_counters();
	wui.get_statistics().clear_labels();
	wui.get_statistics().set_treated_label("item(s) treated");
	wui.get_statistics().set_skipped_label("item(s) excluded by filters");
	wui.get_statistics().set_errored_label("items(s) with error");

	    // launching libdar in a separated thread
	arch_test.run();
	current_thread = & arch_test;
	libdar_running = true;
    }
    else if(event_name == user_interface::start_create)
    {
	if(libdar_running)
	    throw WEBDAR_BUG;
	if(current_thread != NULL)
	    throw WEBDAR_BUG;

	    // providing libdar::parameters
	arch_create.set_user_interaction(wui.get_user_interaction());
	arch_create.set_archive_path(wui.get_parametrage().get_archive_path());
	arch_create.set_archive_basename(wui.get_parametrage().get_archive_basename());
	arch_create.set_archive_extension(EXTENSION);
	if(wui.get_parametrage().get_creating_options().has_reference())
	    arch_create.set_archive_options_reference(
		wui.get_parametrage().get_creating_options().get_reference().get_archive_path(),
		wui.get_parametrage().get_creating_options().get_reference().get_archive_basename(),
		EXTENSION,
		wui.get_parametrage().get_creating_options().get_reference().get_read_options());
	else
	    arch_create.clear_archive_options_reference();
	arch_create.set_fs_root(wui.get_parametrage().get_fs_root());
	arch_create.set_archive_options_create(wui.get_parametrage().get_creating_options().get_options());
	arch_create.set_progressive_report(wui.get_statistics().get_libdar_statistics());

	    // resetting counters and logs
	wui.get_user_interaction().clear();
	wui.get_statistics().clear_counters();
	wui.get_statistics().clear_labels();
	wui.get_statistics().set_treated_label("item(s) treated");
	wui.get_statistics().set_skipped_label("item(s) excluded by filters");
	wui.get_statistics().set_errored_label("items(s) with error");

	    // launching libdar in a separated thread
	arch_create.run();
	current_thread = & arch_create;
	libdar_running = true;
    }
    else if(event_name == user_interface::start_isolate)
    {

    }
    else if(event_name == user_interface::start_merge)
    {

    }
    else
	throw WEBDAR_BUG; // what's that event !?!
}

    //////////////////////////
    //  class fields and methods implementation
    //

mutex session::lock_running;
map<string, session::table> session::running_session;

unsigned int session::get_num_session()
{
    unsigned int ret = 0;

    lock_running.lock();
    try
    {
	ret = running_session.size();
    }
    catch(...)
    {
        lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    return ret;
}

unsigned int session::get_num_session(const string & user)
{
    unsigned int ret = 0;
    map<string, table>::iterator it;

    lock_running.lock();
    try
    {
	for(it = running_session.begin(); it != running_session.end(); ++it)
	{
	    if(it->second.owner == user)
		++ret;
	}
    }
    catch(...)
    {
        lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    return ret;
}


vector<session::session_summary> session::get_summary()
{
    vector<session_summary> ret;
    map<string, table>::iterator it;
    session_summary tmp;

    lock_running.lock();
    try
    {
	it = running_session.begin();
	while(it != running_session.end())
	{
	    if(it->second.reference == NULL)
		throw WEBDAR_BUG;
	    tmp = publish(it);
	    ret.push_back(tmp);
	    ++it;
	}
    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    return ret;
}

bool session::get_session_info(const std::string & session_ID, session_summary & val)
{
    bool ret = false;
    map<string, table>::iterator it;

    lock_running.lock();
    try
    {
	it = running_session.find(session_ID);
	if(it != running_session.end())
	{
	    val = publish(it);
	    if(val.session_ID != session_ID)
		throw WEBDAR_BUG;
	    ret = true;
	}

    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    return ret;
}

string session::create_new(const string & owner)
{
    table entry;
    string sessID;
    session *obj = new (nothrow) session();

    if(obj == NULL)
	throw exception_memory();

    try
    {
	entry.clear();

	lock_running.lock();
	try
	{
		// looking whether the new session_ID is not already used
	    do
	    {
		sessID = webdar_tools_generate_random_string(SESSION_ID_WIDTH);
	    }
	    while(running_session.find(sessID) != running_session.end());

	    obj->set_session_id(sessID);
	    entry.owner = owner;
	    entry.reference = obj;

	    running_session[sessID] = entry;
	}
	catch(...)
	{
	    lock_running.unlock();
	    throw;
	}
	lock_running.unlock();
    }
    catch(...)
    {
	if(obj != NULL)
	    delete obj;
	throw;
    }

    return sessID;
}

session *session::acquire_session(const std::string & session_ID)
{
    session *ret = NULL;
    map<string,table>::iterator it;

    lock_running.lock();

    try
    {
	it = running_session.find(session_ID);
	if(it != running_session.end() && !(it->second.closing))
	{
	    if(it->second.reference == NULL)
		throw WEBDAR_BUG;
	    ret = it->second.reference;
	    ++(it->second.ref_given);
	}
    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    if(ret != NULL)
    {
	ret->lock_wui.lock(); // eventually waiting for another thread to release the mutex
	ret->tid = pthread_self();
    }

    return ret;
}


void session::release_session(session *sess)
{
    map<string,table>::iterator it;

    if(sess == NULL)
	throw WEBDAR_BUG;

    lock_running.lock();
    try
    {
	it = running_session.begin();

	while(it != running_session.end() && it->second.reference != sess)
	    ++it;

	    // checks
	if(it == running_session.end())
	    throw WEBDAR_BUG; // releasing an unknown session !?!
	sess->check_caller();

	    // all check passed, we can proceed
	--(it->second.ref_given);
	sess->lock_wui.unlock();
	if(it->second.ref_given == 0 && it->second.closing)
	{
	    if(it->second.reference == NULL)
		throw WEBDAR_BUG;
	    if(it->second.reference->has_working_server())
		throw WEBDAR_BUG;
	    delete it->second.reference;
	    running_session.erase(it);
	}
    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }
    lock_running.unlock();
}


bool session::close_session(const string & session_ID)
{
    lock_running.lock();
    map<string, table>::iterator it;
    bool ret = false;

    try
    {
	it = running_session.find(session_ID);
	if(it != running_session.end())
	{
	    if(!it->second.closing)
	    {
		it->second.closing = true;
		if(it->second.ref_given == 0)
		{
		    if(it->second.reference != NULL)
		    {
			if(it->second.reference->libdar_running)
			{
			    if(it->second.reference->current_thread != NULL)
			    {
				if(it->second.reference->current_thread->is_running())
				    it->second.reference->current_thread->kill();
				    //
				it->second.reference->current_thread->join();
				    // this later call may propagate exceptions
				it->second.reference->current_thread = NULL;
			    }
			}
			delete it->second.reference;
			running_session.erase(it);
		    }
		    else
			throw WEBDAR_BUG;
		}
		    // else we the object will be destroyed when no more reference will point it
		ret = true; // session will be destroyed as soon as possible
	    }
	    else
		ret = true; // this session end has already been asked
	}
	else
	    ret = false;
    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }

    lock_running.unlock();

    return ret;
}

session::session_summary session::publish(std::map<std::string, table>::iterator it)
{
    session_summary ret;

    if(it->second.reference == NULL)
	throw WEBDAR_BUG;
    ret.clear();
    ret.owner = it->second.owner;
    ret.session_ID = it->first;
    ret.locked = it->second.reference->has_working_server();
    ret.libdar_running = it->second.reference->libdar_running;
    ret.closing = it->second.closing;

    return ret;
}
