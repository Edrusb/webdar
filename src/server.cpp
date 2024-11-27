/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2024 Denis Corbin
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
#include "my_config.h"
extern "C"
{
#if HAVE_ERRNO_H
#include <errno.h>
#endif
}

    // C++ system header files
#include <new>

    // libraries header files
#include <libthreadar/libthreadar.hpp>


    // webdar headers
#include "exceptions.hpp"
#include "central_report.hpp"
#include "responder.hpp"
#include "server.hpp"
#include "webdar_tools.hpp"
#include "error_page.hpp"
#include "challenge.hpp"
#include "choose.hpp"
#include "static_object_library.hpp"
#include "global_parameters.hpp"
#include "disconnected_page.hpp"

using namespace std;

static string get_session_ID_from(const request & req);

server::server(const shared_ptr<central_report> & log,
	       const shared_ptr<const authentication> & auth,
	       unique_ptr<proto_connexion> & source) :
    src(source, log),
    can_keep_session(true),
    locked_session(nullptr),
    ignore_auth(no_ignore)
{
    if(!log)
	throw WEBDAR_BUG;
    rep = log;
    if(!auth)
	throw WEBDAR_BUG;
    authsrc = auth;
}

void server::inherited_run()
{
    answer ans;
    string session_ID = "";
    uri url;
    session *sess = nullptr;
    challenge chal(authsrc);
    session::session_summary info;
    string user;
    disconnected_page disconned;
    choose chooser;
    bool initial = true;

    try
    {
	try
	{
		// outer while loop

	    while(src.get_status() == proto_connexion::connected)
	    {
		try
		{
			// inner while loop

		    while(sess == nullptr ||
			  (src.get_next_request_uri(url)
			   && webdar_tools_get_session_ID_from_URI(url) == sess->get_session_ID()))
		    {
			cancellation_checkpoint();
			try
			{
			    const request & req = src.get_request(); // pending for the next request to come
			    ans.clear();

				// extract session info if any
			    session_ID = get_session_ID_from(req);

			    if(session_ID == STATIC_PATH_ID)
			    {
				try
				{
				    const static_object *obj = nullptr;
				    chemin tmp = req.get_uri().get_path();

				    string objname = tmp.back();
				    tmp.pop_back();
				    if(tmp.front() != STATIC_PATH_ID)
					throw WEBDAR_BUG;
				    if(tmp.size() != 1)
					throw exception_range("local exception to trigger an answer with STATUS_CODE_NOT_FOUND");
				    obj = static_object_library::find_object(objname);
				    if(obj == nullptr)
					throw WEBDAR_BUG;
				    ans = obj->give_answer();
				}
				catch(exception_range & e)
				{
				    ans.set_reason("unknown static object");
				    ans.set_status(STATUS_CODE_NOT_FOUND);
				}
			    }
			    else // not a path to a static object
			    {
				    // show the disconnected page with uri cleaned from session info
				if(ignore_auth == ignore_auth_redir)
				{
				    ignore_auth = ignore_auth_steady;
				    disconned.set_redirect(false);
				    ans = disconned.give_answer(req);
				}
				    // check whether the session is authenticated
				else if(!chal.is_an_authoritative_request(req, user)
					|| ignore_auth == ignore_auth_steady)
				{
					// ask for user authentication
				    ans = chal.give_answer(req);
				    ignore_auth = no_ignore;
				}
				else // session authenticated for user "user"
				{
				    chooser.set_owner(user);

				    if(!session::get_session_info(session_ID, info)
				       || info.locked
				       || info.owner != user)
				    {
					    // session in URL is not valid for that user

					    // try creating a first session if just connected
					    // and no other session was created so far for that
					    // user, then go to that session (refresh in the provided answer)
					if(!initial
					   || !session::create_new_session(user,
									   true,
									   req,
									   ans))

					{
						// else display the list of available sessions for that user

					    initial = false;
					    ans = chooser.give_answer(req);
					    if(chooser.disconnection_requested())
					    {
						ignore_auth = ignore_auth_redir;
						disconned.set_redirect(true);
						ans = disconned.give_answer(req);
					    }
					}
				    }
				    else // this is a valid session for that user
				    {
					if(sess != nullptr && sess->get_session_ID() != session_ID)
					    throw WEBDAR_BUG;

					if(sess == nullptr)
					{
					    sess = session::acquire_session(session_ID);
					    if(sess == nullptr)
						throw WEBDAR_BUG;
					}
					    // obtaining the answer from the session object
					ans = sess->give_answer(req);
					if(sess->disconnection_requested())
					{
					    ignore_auth = ignore_auth_redir;
					    disconned.set_redirect(true);
					    ans = disconned.give_answer(req);
					}
				    }
				}
			    }

				// send back the anwser
			    src.send_answer(ans);
			}
			catch(exception_signal & e)
			{
				// nothing to do
			}
			catch(exception_input & e)
			{
				// nothing to do
			}

		    } // end of the inner while loop

			// release the lock for the current session
		    if(sess != nullptr)
		    {
			session::release_session(sess);
			sess = nullptr;
		    }
		}
		catch(...)
		{
			// release the lock for the current session
		    if(sess != nullptr)
		    {
			session::release_session(sess);
			sess = nullptr;
		    }
		    src.close();
		    throw;
		}
	    } // end of outler while loop
	}
	catch(exception_range & e)
	{
	    rep->report(notice, string("Server thread ending: ") + e.get_message());
	}
    }
    catch(...)
    {
	end_all_peers();
	throw;
    }
    end_all_peers();
}

void server::end_all_peers()
{
    reference* ptr = nullptr;

    reset_read_peers();
    while(read_next_peer(ptr))
	break_peer_with(ptr);
	// there should only be at most one peer: the server_pool that if we have been created by such object
}


static string get_session_ID_from(const request & req)
{
    return webdar_tools_get_session_ID_from_URI(req.get_uri());
}
