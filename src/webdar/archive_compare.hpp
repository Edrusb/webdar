#ifndef ARCHIVE_COMPARE_HPP
#define ARCHIVE_COMPARE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "thread.hpp"
#include "web_user_interaction.hpp"

class archive_compare : public thread
{
public:
    archive_compare();

    void set_user_interaction(web_user_interaction & ref) { ui = ref; };
    void set_archive_path(const std::string & val);
    void set_archive_basename(const std::string & val) { basename = val; };
    void set_archive_options_read(const libdar::archive_options_read & val) { read_opt = val; };
    void set_fs_root(const std::string & val);
    void set_archive_options_compare(const libdar::archive_options_diff & val) { diff_opt = val; };
    void set_progressive_report(libdar::statistics *ptr) { progressive_report = ptr; };

protected:
	/// inherited from class thread
    virtual void inherited_run();

private:
    web_user_interaction ui;
    libdar::path archpath;
    std::string basename;
    libdar::path fs_root;
    libdar::archive_options_read read_opt;
    libdar::archive_options_diff diff_opt;
    libdar::statistics *progressive_report;
};

#endif
