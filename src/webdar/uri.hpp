#ifndef URI_HPP
#define URI_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <vector>


    // webdar headers


    /////////////////////////////////////////////////////////////////////////////////////
    /// uri type holds the splitted list of the scheme / hostname / path
    ///
    /// the first member is always the scheme
    /// the second member is always the host part of the URL (or an empty string for local and relative URL)
    /// the further members is the path of the URL

class uri
{
public:
	/// constructors
    uri() { clear(); };
    uri(const std::string & res) { read(res); };

	// default copy constructor is fine
	// default assignment operator is fine too
	// no need of destructor here

	/// convert an uri from a string
    void read(const std::string & res);

	/// clear the uri (empty uri)
    void clear() { url.clear(); };

	/// number of part that compose the uri
    unsigned int size() const { return url.size(); };

	/// retrieve a part from the uri
    const std::string & operator[] (unsigned int item) const { return url[item]; };

	/// retrieve a part from the uri
    std::string & operator[] (unsigned int item) { return url[item]; };

	/// rebuid the uri as a single string
    const std::string get_string() const;

private:
    std::vector<std::string> url;

};

#endif
