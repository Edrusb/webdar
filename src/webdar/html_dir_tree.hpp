#ifndef HTML_DIR_TREE_HPP
#define HTML_DIR_TREE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <vector>

    // webdar headers
#include "html_div.hpp"
#include "events.hpp"
#include "chemin.hpp"
#include "archive_init_list.hpp"
#include "html_button.hpp"


class html_dir_tree: public html_div, public actor
{
public:
    html_dir_tree(const std::string & chemin); // minimal constructeur, must use set_source() asap
    html_dir_tree(const archive_init_list * ref, const std::string & chemin);
    html_dir_tree(const html_dir_tree & ref):shrink("",""),expand("",""),name("",""), nosubdir("","") { throw WEBDAR_BUG; };
    const html_dir_tree & operator = (const html_dir_tree & ref) { throw WEBDAR_BUG; };
    ~html_dir_tree() { clear(); };

	/// clear information learnt from a previously opened archive
    void clear();

    void set_source(const archive_init_list *ref);
    void set_css_classid(const std::string & classid);

    void go_shrink();
    void go_expand();

	/// inherited from actor
    virtual void on_event(const std::string & event_name);

	/// inherite from html_div
    virtual std::string get_body_part(const chemin & path,
				      const request & req);


private:
    static const std::string event_shrink;
    static const std::string event_expand;
    static const std::string event_click;

    chemin my_path;
    const archive_init_list *src;
    bool info_read;
    bool has_sub;
    std::string last_classid;

	// HTML controls
    html_button shrink; //< alternatively visible button
    html_button expand; //< alternatively visible button
    html_button nosubdir; //< "shown" when no subdir exist
    html_button name;   //< name of the directory
    html_div for_subdirs; //< where take place subdirs
    bool visibility_has_changed; //< whether we need to rebuild the body_builder answer
    std::vector<html_dir_tree *> subdirs; //< subdirectories of the tree

    void init(const std::string & chemin);
    void go_init_indent();
    void set_source_no_expand(const archive_init_list *ref);
    void go_hide();
};

#endif
