#include "exceptions.hpp"

void throw_as_most_derivated_class(exception_base *ebase)
{
    exception_memory *emem = dynamic_cast<exception_memory *>(ebase);
    exception_bug *ebug = dynamic_cast<exception_bug *>(ebase);
    exception_thread *ethread = dynamic_cast<exception_thread *>(ebase);

    if(emem != NULL)
	throw *emem;
    if(ebug != NULL)
	throw *ebug;
    if(ethread != NULL)
	throw *ethread;

    throw WEBDAR_BUG; // unknown exception
}
