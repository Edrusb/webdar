/*********************************************************************/
// webdar - a web interface to libdar library
// Copyright (C) 2012 Denis Corbin
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// to contact the author : http://dar.linux.free.fr/email.html
/*********************************************************************/

extern "C"
{
#include <pthread.h>
#include <stdlib.h>
}

#include <string>
#include <iostream>
#include <vector>

using namespace std;

////////////////////////////////////////////////////////////////////

class erreur
{
public:
    erreur(const string & msg, int code): mess(msg), val(code) {};

    const string & get_message() const { return mess; };
    int get_val() const { return val; };

private:
    string mess;
    int val;
};

////////////////////////////////////////////////////////////////////

class client
{
public:
    client(): identity(0), counter(0) {};
    client(int id, int max): identity(id), counter(max) {};

    int decrement()
    {
	if(counter == 0)
	    throw erreur("0 already reached!", 0);
	return --counter;
    };

    void set_id(int x) { identity = x; };
    void set_counter(int x) { counter = x; };

    int get_id() const { return identity; };
    int get_counter() const { return counter; };

    pthread_t run()
    {
	pthread_t ret;

	if(pthread_create(&ret, NULL, &client::threading_run, (void *)this) < 0)
	    throw erreur("failed thread creation", 1);
	return ret;
    }

private:
    int identity;
    int counter;

    void threaded_run()
    {
	while(decrement() != 0)
	{
	    cout << "client " << get_id() << " dropped to " << get_counter() << endl;
	    sleep(1);
	}
	cout << "client " << get_id() << " TERMINE" << endl;
    };


    static void *threading_run(void *obj)
    {
	client *casted = reinterpret_cast<client *>(obj);
	if(casted == NULL)
	    throw erreur("failed conversion from void * to pointer to object", 0);
	casted->threaded_run();
	return 0;
    };
};

////////////////////////////////////////////////////////////////////

void go_counter(client *o)
{
    if(o == 0)
	throw erreur("null given as argument", 1);

    while(o->decrement() != 0)
    {

	sleep(1);
    }

}

const int MAX = 10;
client tableau[MAX];
pthread_t tache[MAX];

int main()
{
    try
    {
	for(int i = 0; i < MAX; ++i)
	{
	    tableau[i].set_id(i+1);
	    tableau[i].set_counter((rand() % 10) + 2);
	    tache[i] = tableau[i].run();
	}

	for(int i = 0; i < MAX; ++i)
	{
	    void *ret;

	    cout << "main thread waiting for thread " << tache[i] << endl;
	    (void) pthread_join(tache[i], &ret);
	}
    }
    catch(erreur & e)
    {
	cout << "uncaught exception of type erreur: " << e.get_message() << " code = " << e.get_val() << endl;
    }
    catch(...)
    {
	cout << "UNKNOWN uncaught exception" << endl;
    }
}

