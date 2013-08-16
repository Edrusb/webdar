#ifndef ARCHIVE_INIT_LIST_HPP
#define ARCHIVE_INIT_LIST_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <dar/libdar.hpp>

    // webdar headers
#include "thread.hpp"
#include "web_user_interaction.hpp"

class archive_init_list : public thread
{
public:
    archive_init_list(): archpath("/") { ptr = NULL; };
    archive_init_list(const archive_init_list & ref): archpath(ref.archpath) { throw WEBDAR_BUG; };
    const archive_init_list & operator = (const archive_init_list & ref) { throw WEBDAR_BUG; };
    ~archive_init_list() { close_archive(); };


	/// set the user interaction to report on when running the thread
    void set_user_interaction(web_user_interaction & ref) { ui = ref; };
    void set_archive_path(const std::string & val);
    void set_archive_basename(const std::string & val) { basename = val; };
    void set_archive_options_read(const libdar::archive_options_read & val) { read_opt = val; };


	/// once the thread has been ran and has finished, we end up with an opened archive
	///
	/// \return true if the archive is available false if a failure occured and no archive available
    bool opened() const { return ptr != NULL; };

	/// obtains contents from the opened archive in the calling thread
    const std::vector<libdar::list_entry> get_children_in_table(const std::string & dir) const;
    bool has_subdirectory(const std::string & dir) const;

	/// close the opened archive run in the calling thread
    void close_archive() { if(ptr != NULL) { delete ptr; ptr = NULL; } };

protected:
	/// inherited from class thread
    virtual void inherited_run();

private:
    web_user_interaction ui;
    libdar::path archpath;
    std::string basename;
    libdar::archive_options_read read_opt;
    libdar::archive *ptr; //< allocated archive object or NULL if none allocated

};

#endif
