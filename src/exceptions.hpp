/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2024 Denis Corbin
//
// This file is part of Webdar
//
//  Webdar is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Webdar is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
//
//----
//  to contact the author: dar.linux@free.fr
/*********************************************************************/

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

    // C system header files
extern "C"
{
#include <string.h>
}

    // C++ system header files
#include <string>
#include <new>

    // libraries header files
#include <dar/libdar.hpp>


    /// pure virtual class parent of all webdar exceptions

class exception_base
{
public:
    exception_base(const std::string & x_msg) {msg = x_msg; };

    virtual ~exception_base() {};

    const std::string & get_message() const { return msg; };
    void change_message(const std::string & x_msg) { msg = x_msg; };

    virtual exception_base *clone() const = 0;

private:
    std::string msg;
};



    ///  exception used to report memory allocation failures

class exception_memory : public exception_base
{
public:
    exception_memory() : exception_base("lack of memory") {};

protected:
    virtual exception_base *clone() const override;
};

template<class T> exception_base *cloner(void * const ptr) { exception_base *ret = new (std::nothrow) T(*(reinterpret_cast<T const *>(ptr))); if(ret == nullptr) throw exception_memory(); return ret; };

inline exception_base * exception_memory::clone() const { return cloner<exception_memory>((void *)this); };

    /// exception used to report webdar internal bugs

#define WEBDAR_BUG exception_bug(__FILE__, __LINE__)

    /// class used to signal bug condition

class exception_bug : public exception_base
{
public:
    exception_bug(const std::string & file, int line) : exception_base(std::string("WEBDAR: BUG MET IN File ") + file + std::string(" line ") + std::to_string(line)) {};

protected:
    virtual exception_base *clone() const override { return cloner<exception_bug>((void *)this); };
};


    /// exception used to report operating system errors

class exception_system : public exception_base
{
public:
    exception_system(const std::string & context, int error_code);

protected:
    virtual exception_base *clone() const override { return cloner<exception_system>((void *)this); };
};

    /// exception used to report out or range value or argument

class exception_range : public exception_base
{
public:
    exception_range(const std::string & msg): exception_base(msg) {};

protected:
    virtual exception_base *clone() const override { return cloner<exception_range>((void *)this); };
};

    /// exception used to report an request error from the http client
    ///
    /// \note at the difference of the exception_range type, this exception
    /// is used when the user request is syntaxically correct but a field value has an
    /// unexpected value. This implies that we can and must answer to by a error HTTP answer
    /// which code and reason phrase is carried by this exception. Further request can
    /// processed on the same connexion.
class exception_input : public exception_base
{
public:
    exception_input(const std::string & msg, unsigned int error_code): exception_base(msg) {err = error_code; };
    unsigned int get_error_code() const { return err; };

protected:
    virtual exception_base *clone() const override { return cloner<exception_input>((void *)this); };

private:
    unsigned int err;
};


    /// exception used to report an non-implemented feature

class exception_feature : public exception_base
{
public:
    exception_feature(const std::string & feature_name): exception_base(std::string("Unimplemented feature: ") + feature_name) {};

protected:
    virtual exception_base *clone() const override { return cloner<exception_feature>((void *)this); };
};

    /// transcription of libdar exception into the webdar exception type

class exception_libcall : public exception_base
{
public:
    exception_libcall(const libdar::Egeneric & e);

protected:
    virtual exception_base *clone() const override { return cloner<exception_libcall>((void *)this); };
};

    /// class used to signal SSL related errors

class exception_openssl : public exception_base
{
public:
    exception_openssl();

protected:
    virtual exception_base *clone() const override { return cloner<exception_openssl>((void *)this); };

private:
    static constexpr unsigned int ERR_BUF_LEN = 1024;

    static std::string get_ssl_error();
};

extern void throw_as_most_derivated_class(exception_base *ebase);

#endif
