#ifndef PARSER_HPP
#define PARSER_HPP

    // webdar headers
#include "connexion.hpp"
#include "thread.hpp"
#include "central_report.hpp"

#include <list>

class parser: public thread
{
public:
	// constructor & Destructor are intentionally set as private methods

    static bool run_new_parser(central_report *log, connexion *source);
    static void set_max_parser(unsigned int val) { max_parser = val; };
    static void kill_all_parsers();

protected:
    void inherited_run();

private:
    parser(central_report *log, connexion *source);
    parser(const parser & ref) { throw WEBDAR_BUG; };
    const parser & operator = (const parser & ref) { throw WEBDAR_BUG; };
    ~parser();

    connexion *src;      //< object owned by the parser
    central_report *rep; //< just a pointer to an existing object, must not be deleted by "this"

	/// static fields
    static pthread_mutex_t lock_counter;  //< manages access to all static fields
    static unsigned int max_parser;       //< max allowed number of concurrent thread
    static std::list<parser *> instances; //< list of existing parser objects
};

#endif
