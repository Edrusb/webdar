#include "events.hpp"
#include <iostream>

using namespace std;

class test_actor : public actor
{
public:
    test_actor(const string & testor_name) { name = testor_name; };

    void on_event(const std::string & event_name) { cout << "Event " + event_name + " received by " + name << endl; };

private:
    string name;
};

int main()
{
    test_actor *p1 = new test_actor("acteur 1");
    test_actor a2 = string("acteur 2");
    test_actor a3 = string("acteur 3");
    events ev;

    ev.register_name("canal 1-2");
    ev.register_name("canal 2-3");

    ev.record_actor_on_event(p1, "canal 1-2");
    ev.record_actor_on_event(&a2, "canal 1-2");
    ev.record_actor_on_event(&a2, "canal 2-3");
    ev.record_actor_on_event(&a3, "canal 2-3");

    ev.act("canal 1-2");
    ev.act("canal 2-3");

    delete p1;

    ev.act("canal 1-2");
    ev.act("canal 2-3");
}

