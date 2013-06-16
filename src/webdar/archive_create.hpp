#ifndef ARCHIVE_CREATE_HPP
#define ARCHIVE_CREATE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "thread.hpp"
#include "web_user_interaction.hpp"
#include "html_options_create.hpp"

class archive_create : public thread
{
public:
    archive_create(): archpath("/"), fs_root("/") { progressive_report = NULL; };

    void set_user_interaction(web_user_interaction & ref) { ui = ref; };
    void set_archive_path(const std::string & val);
    void set_archive_basename(const std::string & val) { basename = val; };
    void set_archive_extension(const std::string & val) { extension = val; };
    void set_archive_options_create(const libdar::archive_options_create & val) { opt = val; };
    void clear_archive_options_reference() { has_ref = false; };
    void set_archive_options_reference(const std::string & refpath,
				       const std::string & basename,
				       const std::string & extension,
				       const libdar::archive_options_read & readopt);
    void set_fs_root(const std::string & val);
    void set_progressive_report(libdar::statistics *ptr) { progressive_report = ptr; };

protected:
	/// inherited from class thread
    virtual void inherited_run();

private:
    web_user_interaction ui;
    libdar::path archpath;
    std::string basename;
    std::string extension;
    libdar::path fs_root;
    libdar::archive_options_create opt;
    bool has_ref;
    std::string ref_path;
    std::string ref_basename;
    std::string ref_extension;
    libdar::archive_options_read ref_opt;
    libdar::statistics *progressive_report;
};

#endif
