#include "exceptions.hpp"
#include "parser.hpp"

void parser::run_new_parser(connexion *source)
{
	// a implementer
}


parser::parser(connexion *source)
{
    if(src == NULL)
	throw WEBDAR_BUG;
    src = source;
}

parser::~parser()
{
    if(src != NULL)
	delete src;
}

void parser::run()
{

	/// implementation a faire

}
