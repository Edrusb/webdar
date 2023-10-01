/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
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

#ifndef LIBDARER_HPP
#define LIBDARER_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers


    /// \file libdarer.hpp defines libdarer class
    ///
    /// libdarer class is the ancestor of all class that generate
    /// libdar components that are hold in a bibliotheque
    /// for now the libdarer class has not use, but is a place holder
    /// if some common routine, concept, fields would be needed in the
    /// future

class libdarer
{
public:
    libdarer() = default;
    libdarer(const libdarer & ref) = default;
    libdarer(libdarer && ref) noexcept(false) = default;
    libdarer & operator = (const libdarer & ref) = default;
    libdarer & operator = (libdarer && ref) noexcept(false) = default;
    virtual ~libdarer() {};
};

///////////// some prototype of class that would be old in bibliotheque
////// the following class will be moved to their dedicated hpp/cpp module

class libdarer_mask: public libdarer, public jsoner, public bibli_referable, public body_builder
{
public:
    libdarer_mask() = default;
    libdarer_mask(const libdarer_mask & ref) = default;
    libdarer_mask(libdarer_mask && ref) noexcept(false) = default;
    libdarer_mask & operator = (const libdarer_mask & ref) = default;
    libdarer_mask & operator = (libdarer_mask && ref) noexcept(false) = default;
    virtual ~libdarer_mask() {};

	/// inherited class of should implement this method
    virtual std::unique_ptr<libdar::mask> generate_libdar() const = 0;
};

class libdarer_entrepot: public libdarer, public jsoner, public bibli_referable, public body_builder
{
public:
    libdarer_repository() = default;
    libdarer_repository(const libdarer_repository & ref) = default;
    libdarer_repository(libdarer_repository && ref) noexcept(false) = default;
    libdarer_repository & operator = (const libdarer_repository & ref) = default;
    libdarer_repository & operator = (libdarer_repository && ref) noexcept(false) = default;
    virtual ~libdarer_repository() {};

	/// inherited class of should implement this method
    virtual std::unique_ptr<libdar::entrepot> generate_libdar() const = 0;
};

class libdarer_conf_read: public libdarer, public jsoner, public bibli_referable, public body_builder
{
public:
    libdarer_conf_read() = default;
    libdarer_conf_read(const libdarer_conf_read & ref) = default;
    libdarer_conf_read(libdarer_conf_read && ref) noexcept(false) = default;
    libdarer_conf_read & operator = (const libdarer_conf_read & ref) = default;
    libdarer_conf_read & operator = (libdarer_conf_read && ref) noexcept(false) = default;
    virtual ~libdarer_conf_read() {};

	/// inherited class of should implement this method
    virtual std::unique_ptr<libdar::archive_options_read> generate_libdar() const = 0;
};

class libdarer_conf_create: public libdarer, public jsoner, public bibli_referable, public body_builder
{
public:
    libdarer_conf_create() = default;
    libdarer_conf_create(const libdarer_conf_create & ref) = default;
    libdarer_conf_create(libdarer_conf_create && ref) noexcept(false) = default;
    libdarer_conf_create & operator = (const libdarer_conf_create & ref) = default;
    libdarer_conf_create & operator = (libdarer_conf_create && ref) noexcept(false) = default;
    virtual ~libdarer_conf_create() {};

	/// inherited class of should implement this method
    virtual std::unique_ptr<libdar::archive_options_create> generate_libdar() const = 0;
};

class libdarer_conf_isolate: public libdarer, public jsoner, public bibli_referable, public body_builder
{
public:
    libdarer_conf_isolate() = default;
    libdarer_conf_isolate(const libdarer_conf_isolate & ref) = default;
    libdarer_conf_isolate(libdarer_conf_isolate && ref) noexcept(false) = default;
    libdarer_conf_isolate & operator = (const libdarer_conf_isolate & ref) = default;
    libdarer_conf_isolate & operator = (libdarer_conf_isolate && ref) noexcept(false) = default;
    virtual ~libdarer_conf_isolate() {};

	/// inherited class of should implement this method
    virtual std::unique_ptr<libdar::archive_options_isolate> generate_libdar() const = 0;
};

class libdarer_conf_merge: public libdarer, public jsoner, public bibli_referable, public body_builder
{
public:
    libdarer_conf_merge() = default;
    libdarer_conf_merge(const libdarer_conf_merge & ref) = default;
    libdarer_conf_merge(libdarer_conf_merge && ref) noexcept(false) = default;
    libdarer_conf_merge & operator = (const libdarer_conf_merge & ref) = default;
    libdarer_conf_merge & operator = (libdarer_conf_merge && ref) noexcept(false) = default;
    virtual ~libdarer_conf_merge() {};

	/// inherited class of should implement this method
    virtual std::unique_ptr<libdar::archive_options_merge> generate_libdar() const = 0;
};

class libdarer_conf_extract: public libdarer, public jsoner, public bibli_referable, public body_builder
{
public:
    libdarer_conf_extract() = default;
    libdarer_conf_extract(const libdarer_conf_extract & ref) = default;
    libdarer_conf_extract(libdarer_conf_extract && ref) noexcept(false) = default;
    libdarer_conf_extract & operator = (const libdarer_conf_extract & ref) = default;
    libdarer_conf_extract & operator = (libdarer_conf_extract && ref) noexcept(false) = default;
    virtual ~libdarer_conf_extract() {};

	/// inherited class of should implement this method
    virtual std::unique_ptr<libdar::archive_options_extract> generate_libdar() const = 0;
};

class libdarer_conf_listing: public libdarer, public jsoner, public bibli_referable, public body_builder
{
public:
    libdarer_conf_listing() = default;
    libdarer_conf_listing(const libdarer_conf_listing & ref) = default;
    libdarer_conf_listing(libdarer_conf_listing && ref) noexcept(false) = default;
    libdarer_conf_listing & operator = (const libdarer_conf_listing & ref) = default;
    libdarer_conf_listing & operator = (libdarer_conf_listing && ref) noexcept(false) = default;
    virtual ~libdarer_conf_listing() {};

	/// inherited class of should implement this method
    virtual std::unique_ptr<libdar::archive_options_listing> generate_libdar() const = 0;
};

class libdarer_conf_diff: public libdarer, public jsoner, public bibli_referable, public body_builder
{
public:
    libdarer_conf_diff() = default;
    libdarer_conf_diff(const libdarer_conf_diff & ref) = default;
    libdarer_conf_diff(libdarer_conf_diff && ref) noexcept(false) = default;
    libdarer_conf_diff & operator = (const libdarer_conf_diff & ref) = default;
    libdarer_conf_diff & operator = (libdarer_conf_diff && ref) noexcept(false) = default;
    virtual ~libdarer_conf_diff() {};

	/// inherited class of should implement this method
    virtual std::unique_ptr<libdar::archive_options_diff> generate_libdar() const = 0;
};

class libdarer_conf_test: public libdarer, public jsoner, public bibli_referable, public body_builder
{
public:
    libdarer_conf_test() = default;
    libdarer_conf_test(const libdarer_conf_test & ref) = default;
    libdarer_conf_test(libdarer_conf_test && ref) noexcept(false) = default;
    libdarer_conf_test & operator = (const libdarer_conf_test & ref) = default;
    libdarer_conf_test & operator = (libdarer_conf_test && ref) noexcept(false) = default;
    virtual ~libdarer_conf_test() {};

	/// inherited class of should implement this method
    virtual std::unique_ptr<libdar::archive_options_test> generate_libdar() const = 0;
};

class libdarer_conf_repair: public libdarer, public jsoner, public bibli_referable, public body_builder
{
public:
    libdarer_conf_repair() = default;
    libdarer_conf_repair(const libdarer_conf_repair & ref) = default;
    libdarer_conf_repair(libdarer_conf_repair && ref) noexcept(false) = default;
    libdarer_conf_repair & operator = (const libdarer_conf_repair & ref) = default;
    libdarer_conf_repair & operator = (libdarer_conf_repair && ref) noexcept(false) = default;
    virtual ~libdarer_conf_repair() {};

	/// inherited class of should implement this method
    virtual std::unique_ptr<libdar::archive_options_repair> generate_libdar() const = 0;
};


    /// this class is not expected to directly provide a libdar object, depending on
    /// the class that refers to it, (libdarer_conf_create, libdarer_conf_repair,...) the content
    /// of that class will be copied to the libdar::archive_option_create or libdar::archive_option_repair
    /// for the field that exist in the resulting libdar object

class libdarer_conf_common: public jsoner, public bibli_referable, public body_builder
{
public:
    libdarer_conf_common() = default;
    libdarer_conf_common(const libdarer_conf_common & ref) = default;
    libdarer_conf_common(libdarer_conf_common && ref) noexcept(false) = default;
    libdarer_conf_common & operator = (const libdarer_conf_common & ref) = default;
    libdarer_conf_common & operator = (libdarer_conf_common && ref) noexcept(false) = default;
    virtual ~libdarer_conf_common() {};
};




#endif
