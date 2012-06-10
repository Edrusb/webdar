#include <string>
#include <dar/tools.hpp>

class exception_base
{
public:
    exception_base(const std::string & x_msg) {msg = x_msg; };

    virtual ~exception_base() {};

    const std::string & get_message() const { return msg; };

    virtual exception_base *clone() const = 0;

private:
    std::string msg;
};

class exception_memory;

template<class T> exception_base *cloner(void * const ptr) { exception_base *ret = new T(*(reinterpret_cast<T const *>(ptr))); if(ret == NULL) throw exception_memory(); return ret; };

class exception_memory : public exception_base
{
public:
    exception_memory() : exception_base("lack of memory") {};

protected:
    virtual exception_base *clone() const { return cloner<exception_memory>((void *)this); };
};


#define WEBDAR_BUG exception_bug(__FILE__, __LINE__)

class exception_bug : public exception_base
{
public:
    exception_bug(const std::string & file, int line) : exception_base(libdar::tools_printf(gettext("WEBDAR: BUG MET IN File %S line %d"), &file, line)) {};

protected:
    virtual exception_base *clone() const { return cloner<exception_bug>((void *)this); };
};



class exception_thread : public exception_base
{
public:
    exception_thread(const std::string & x_msg) : exception_base(x_msg) {};

protected:
    virtual exception_base *clone() const { return cloner<exception_thread>((void *)this); };
};

extern void throw_as_most_derivated_class(exception_base *ebase);
