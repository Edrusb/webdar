#ifndef ARCHIVE_MERGE_HPP
#define ARCHIVE_MERGE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "thread.hpp"
#include "web_user_interaction.hpp"
#include "html_options_merge.hpp"

class archive_merge : public thread
{
public:
    archive_merge(): archpath("/") { progressive_report = NULL; };

    void set_user_interaction(web_user_interaction & ref) { ui = ref; };
    void set_archive_path(const std::string & val);
    void set_archive_basename(const std::string & val) { basename = val; };
    void set_archive_extension(const std::string & val) { extension = val; };
    void set_archive_options_merge(const libdar::archive_options_merge & val) { opt = val; };
    void clear_archive_reference() { has_ref = false; };
    void set_archive_reference(const std::string & refpath,
			       const std::string & basename,
			       const std::string & extension,
			       const libdar::archive_options_read & readopt);
    void clear_archive_options_auxilliary() { has_aux = false; };
    void set_archive_options_auxilliary(const std::string & refpath,
					const std::string & basename,
					const std::string & extension,
					const libdar::archive_options_read & readopt);
    void set_progressive_report(libdar::statistics *ptr) { progressive_report = ptr; };

protected:
	/// inherited from class thread
    virtual void inherited_run();

private:
    web_user_interaction ui;
    libdar::path archpath;
    std::string basename;
    std::string extension;
    libdar::archive_options_merge opt;
    bool has_ref;
    std::string ref_path;
    std::string ref_basename;
    std::string ref_extension;
    libdar::archive_options_read ref_opt;
    bool has_aux;
    std::string aux_path;
    std::string aux_basename;
    std::string aux_extension;
    libdar::archive_options_read aux_opt;
    libdar::statistics *progressive_report;
};

#endif
