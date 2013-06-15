#ifndef ARCHIVE_TEST_HPP
#define ARCHIVE_TEST_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <string>

    // webdar headers
#include "thread.hpp"
#include "web_user_interaction.hpp"

class archive_test : public thread
{
public:
    archive_test(): archpath("/") { progressive_report = NULL; };

    void set_user_interaction(web_user_interaction & ref) { ui = ref; };
    void set_archive_path(const std::string & val);
    void set_archive_basename(const std::string & val) { basename = val; };
    void set_archive_options_read(const libdar::archive_options_read & val) { read_opt = val; };
    void set_archive_options_test(const libdar::archive_options_test & val) { test_opt = val; };
    void set_progressive_report(libdar::statistics *ptr) { progressive_report = ptr; };

protected:
	/// inherited from thread class
    virtual void inherited_run();

private:
    web_user_interaction ui;
    libdar::path archpath;
    std::string basename;
    libdar::archive_options_read read_opt;
    libdar::archive_options_test test_opt;
    libdar::statistics *progressive_report;
};

#endif
