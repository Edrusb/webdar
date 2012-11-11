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
    /// implements path objects

class chemin
{
public:
	/// constucts an empty path
    chemin() { index = 0; };
    chemin(const std::string & path);

	/// default copy constructor is OK

	/// default assignement operator is OK too

	/// default destructor is also fine here


	/// set the path as an empty path
    void clear() { members.clear(); index = 0; };

	/// compaires two path
    bool operator == (const chemin & ref) const { return members == ref.members; };

	/// concatenate a path to this object
    void operator += (const chemin & ref);

	/// add a member at the end of the path
    void push_back(const std::string & x);

	/// get the first member of the path
    std::string front() const { if(empty()) throw exception_range("cannot get front from an empty chemin"); return members.front(); };

	/// removes the first member of the path from this object
	///
	/// \note throws an exception if the path is empty
    void pop_front();

	/// removes the last member of the path from this object
    void pop_back();

	/// returns the size of the path in number of members
    unsigned int size() const { return members.size(); };

	/// return true if the path is an empty path (having no members)
    bool empty() const { return members.size() == 0; };


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
    std::string display() const;

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
