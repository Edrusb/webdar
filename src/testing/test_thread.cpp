#include <string>
#include <iostream>

#include "thread.hpp"

using namespace std;

class test_thread : public thread
{
public:
    test_thread(const string & id, unsigned int countdown) { count = countdown; identity = id; };

    void reset(unsigned int val) { count = val; };

protected:
    void inherited_run();


private:
    unsigned int count;
    string identity;
};

void test_thread::inherited_run()
{
    while(count > 0)
    {
	cout << "identity : " << count << endl;
	--count;
	sleep(1);
    }
}

void f1();

int main()
{
    f1();
}

void f1()
{
    test_thread t1 = test_thread("bonjour", 10);
    test_thread t2 = test_thread("coucou", 5);
    pthread_t id;

    t1.kill();
    t1.join();
    cout << t1.is_running(id) << endl;

    t1.run();
    t2.run();
    t1.join();
    t2.join();

    t1.reset(10);
    t2.reset(20);
    t1.run();
    t2.run();
    t2.kill();
    t1.join();
    t2.join();
}
