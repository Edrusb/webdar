
    // webdar headers
#include "exceptions.hpp"

exception_system::exception_system(const std::string & context, int error_code) : exception_base("")
{
    const unsigned int SIZE = 300;
    char buffer[SIZE];
#if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE
    int ret = strerror_r(error_code, buffer, SIZE);
    buffer[SIZE - 1] = '\0'; // that does not hurt
    if(ret == 0) // (successfull call)
	change_message(context + ": " + buffer);
    else
#endif
	change_message(libdar::tools_printf("System failed with error code %d", error_code));
}

void throw_as_most_derivated_class(exception_base *ebase)
{
    exception_memory *emem = dynamic_cast<exception_memory *>(ebase);
    exception_bug *ebug = dynamic_cast<exception_bug *>(ebase);
    exception_thread *ethread = dynamic_cast<exception_thread *>(ebase);
    exception_system *esys = dynamic_cast<exception_system *>(ebase);
    exception_range *erange = dynamic_cast<exception_range *>(ebase);
    exception_input *input = dynamic_cast<exception_input *>(ebase);
    exception_feature *feature = dynamic_cast<exception_feature *>(ebase);
    exception_libcall *libcall = dynamic_cast<exception_libcall *>(ebase);

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
    if(input != NULL)
	throw *input;
    if(feature != NULL)
	throw *feature;
    if(libcall != NULL)
	throw *libcall;

    throw WEBDAR_BUG; // unknown exception
}
