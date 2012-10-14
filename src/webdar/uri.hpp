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
    uri() { url.clear(); };
	// default copy constructor is fine
	// default assignment operator is fine too
	// no need of destructor here

    unsigned int size() const { return url.size(); };
    void clear() { url.clear(); };

    const std::string & operator[] (unsigned int item) const { return url[item]; };
    std::string & operator[] (unsigned int item) { return url[item]; };
    const std::string get_string() const;


private:
    std::vector<std::string> url;
};

#endif
