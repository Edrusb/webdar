#ifndef HTML_STATISTICS_HPP
#define HTML_STATISTICS_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <dar/libdar.hpp>


    // webdar headers
#include "body_builder.hpp"
#include "html_table.hpp"
#include "html_text.hpp"

class html_statistics : public body_builder
{
public:
    html_statistics();

    void clear_counters() { stats.clear(); };
    void clear_labels();

	// fields that do not get a label are not shown
    void set_treated_label(const std::string & label) { treated_lbl.clear(); treated_lbl.add_text(0, label); unbuild(); };
    void set_hard_links_label(const std::string & label) { hard_links_lbl.clear(); hard_links_lbl.add_text(0, label); unbuild(); };
    void set_skipped_label(const std::string & label) { skipped_lbl.clear(); skipped_lbl.add_text(0, label); unbuild(); };
    void set_ignored_label(const std::string & label) { ignored_lbl.clear(); ignored_lbl.add_text(0, label); unbuild(); };
    void set_tooold_label(const std::string & label) { tooold_lbl.clear(); tooold_lbl.add_text(0, label); unbuild(); };
    void set_errored_label(const std::string & label) { errored_lbl.clear(); errored_lbl.add_text(0, label); unbuild(); };
    void set_deleted_label(const std::string & label) { deleted_lbl.clear(); deleted_lbl.add_text(0, label); unbuild(); };
    void set_ea_treated_label(const std::string & label) { ea_deleted_lbl.clear(); ea_deleted_lbl.add_text(0, label); unbuild(); };
    void set_byte_amount_label(const std::string & label) { byte_amount_lbl.clear(); byte_amount_lbl.add_text(0, label); unbuild(); };

	/// the address of the object to be updated by libdar
    libdar::statistics *get_libdar_statistics();

	/// inherited from body_builder class
	///
	/// \note always fields that got a label (using a set_*_label() method)
	/// are displayed.
    virtual std::string get_body_part(const chemin & path,
				      const request & req)
    { return get_body_part_from_all_children(path, req); };

private:
    libdar::statistics stats;

    html_table *table;

    html_text treated_lbl;
    html_text hard_links_lbl;
    html_text skipped_lbl;
    html_text ignored_lbl;
    html_text tooold_lbl;
    html_text errored_lbl;
    html_text deleted_lbl;
    html_text ea_deleted_lbl;
    html_text byte_amount_lbl;

    html_text treated_count;
    html_text hard_links_count;
    html_text skipped_count;
    html_text ignored_count;
    html_text tooold_count;
    html_text errored_count;
    html_text deleted_count;
    html_text ea_deleted_count;
    html_text byte_amount_count;

	/// build the html body_builder tree using only labelled field
    void build();

	/// destroy the table to force a build() at the next cycle
    void unbuild();
};

#endif
