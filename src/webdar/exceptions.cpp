
    // webdar headers
#include "exceptions.hpp"

void throw_as_most_derivated_class(exception_base *ebase)
{
    exception_memory *emem = dynamic_cast<exception_memory *>(ebase);
    exception_bug *ebug = dynamic_cast<exception_bug *>(ebase);
    exception_thread *ethread = dynamic_cast<exception_thread *>(ebase);
    exception_system *esys = dynamic_cast<exception_system *>(ebase);
    exception_range *erange = dynamic_cast<exception_range *>(ebase);

    if(emem != NULL)
	throw *emem;
    if(ebug != NULL)
	throw *ebug;
    if(ethread != NULL)
	throw *ethread;
    if(esys != NULL)
	throw *esys;
    if(erange != NULL)
	throw *erange;

    throw WEBDAR_BUG; // unknown exception
}
