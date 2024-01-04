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

#ifndef CHEMIN_HPP
#define CHEMIN_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <deque>

    // webdar headers
#include "exceptions.hpp"


    /// class chemin definition
    /// \note "chemin" means path in French, this class
    /// implements path objects. There is no relative/absolute
    /// caracteristics in these objects, Only at display() time
    /// one can decide to given a string representing the path as
    /// either absolute or relative.

class chemin
{
public:
	/// constucts an empty path
    chemin() { members.clear(); index = 0; };
    chemin(const std::string & path);
    chemin(const chemin & ref) = default;
    chemin(chemin && ref) noexcept = default;
    chemin & operator = (const chemin & ref) = default;
    chemin & operator = (chemin && ref) noexcept = default;
    ~chemin() = default;


	/// default copy constructor is OK

	/// default assignement operator is OK too

	/// default destructor is also fine here


	/// set the path as an empty path
    void clear() { members.clear(); index = 0; };

	/// compaires two path
    bool operator == (const chemin & ref) const { return members == ref.members; };
    bool operator != (const chemin & ref) const { return !((*this) == ref); };

	/// return true if 'this' is the beginning of ref
    bool is_the_beginning_of(const chemin & ref) const;

	/// concatenate a path to this object
    void operator += (const chemin & ref);

	/// concatenate a path with us another
    chemin operator + (const chemin & ref) const { chemin ret = *this; ret += ref; return ret; };

	/// add a member at the end of the path
    void push_back(const std::string & x);

	/// get the first member of the path

	/// \note if the path is empty() an exception_range is thrown
    std::string front() const { if(empty()) throw exception_range("cannot get front() from an empty chemin"); return members.front(); };

	/// get the last member of the path

	/// \note if the path is empty() an exception_range is thrown
    std::string back() const { if(empty()) throw exception_range("cannot get back() from an empty chemin"); return members.back(); };

	/// removes the first member of the path from this object (root side)
	///
	/// \note throws an exception if the path is empty
    void pop_front();

	/// removes the last member of the path from this object
    void pop_back();

	/// returns the size of the path in number of members
    unsigned int size() const { return members.size(); };

	/// return true if the path is an empty path (having no members)
    bool empty() const { return members.empty(); };


	/// return the index of the path
	///
	/// \note the index is a pointer to a member of the path. Zero is the first member
	/// the index can be modified at will even on const objects, but always points to a valid
	/// member of the path. If the path is empty, playing with index raises an exception
    unsigned int get_index() const { return index; };

	/// set the index to the next member. If the member is the last of the path this call does nothing
    void increase_index() const { if(index < size() - 1) ++(const_cast<chemin *>(this)->index); };

	/// set the index to the previous member. If the member is the first of the path, this call does nothing
    void decrease_index() const { if(index > 0) --(const_cast<chemin *>(this)->index); };

	/// set the index to an arbitrary value. An exception is thrown if the given value is greater or equal the path size
    void set_index(unsigned int val) const;

	/// return a string corresponding to the expected value of an absolute path
    std::string display(bool relative = false) const;

	/// return a compact string to be used as identifier.
	///
	/// \note Two different path give two different strings, two equal path give the same identifier
    std::string namify() const;

	/// access to a given member of the path
	///
	/// \param[in] x index of the path to retrieve. This argument value must range from zero to size-1
	/// \return the requested member of the path
	/// \note if called on an empty path, an exception is thrown.
    const std::string & operator[] (unsigned int x) const;

private:
    std::deque<std::string> members;
    unsigned int index;
};


#endif
