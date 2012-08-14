#ifndef PARSER_HPP
#define PARSER_HPP

#include <libdar/libdar.hpp>

#include "connexion.hpp"

class parser
{
public:

	/// constructor and destructor are made private intentionnaly

    static void run_new_parser(connexion *source);
    static void set_max_parser(libdar::U_I val) { max_parser = val; };

private:
    parser(connexion *source);
    ~parser(); //< destroy the connexion object
    void run(); //< parsing loop

    connexion *src;

	/// static fields

    static bool initialized; //< if not initialized, run_new_parser or set_max_parser set counter to zero (and max_parser to zero (no limit) or "val"), also initialize the mutex and lock it. Once locked, if initialized has became true, this means a race condition occured, esle set it to true (the order of fields initialization is to be review in more details)
    static pthread_mutex_t lock_counter;
    static libdar::U_I counter;
    static libdar::U_I max_parser;
};

#endif
