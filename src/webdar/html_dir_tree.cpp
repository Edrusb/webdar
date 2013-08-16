    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "tokens.hpp"


    //
#include "html_dir_tree.hpp"

using namespace std;

const string html_dir_tree::event_shrink = "html_dir_tree_shrink";
const string html_dir_tree::event_expand = "html_dir_tree_expand";
const string html_dir_tree::event_click = "html_dir_tree_click";

html_dir_tree::html_dir_tree(const std::string & chemin):
    shrink("-", event_shrink),
    expand("+", event_expand),
    name("", event_click),
    nosubdir(" ","")
{
    init(chemin);
}

html_dir_tree::html_dir_tree(const archive_init_list * ref, const std::string & chemin):
    shrink("-", event_shrink),
    expand("+", event_expand),
    name("", event_click),
    nosubdir(" ","")
{
    init(chemin);
    set_source_no_expand(ref);
}

void html_dir_tree::init(const std::string & chemin)
{
    my_path = chemin;
    src = NULL;
    info_read = false;
    has_sub = true; // until checked it has or has not
    last_classid = "";

    if(chemin == "")
	name.change_label("&lt;ROOT&gt;");
    else
	name.change_label(my_path.back());

	// setting css properties
    css_padding("0.1em");
    set_no_CR();

    shrink.css_float(css::fl_left, true);
    shrink.css_float_clear(css::fc_both, true);
    shrink.css_width("2em", false, true);
    shrink.css_text_align(css::al_center, true);
    shrink.css_margin_right("1em", true);
    shrink.css_border_width(css::bd_all, css::bd_medium, true);
    shrink.css_border_style(css::bd_all, css::bd_solid, true);

    nosubdir.css_inherit_from(shrink); // propagate the just defined css value from shrink to nosubdir
    nosubdir.css_border_color(css::bd_all, COLOR_BACK);
    nosubdir.css_height("1em", false);

    shrink.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF);

    expand.css_inherit_from(shrink); // propagate all defined css value from shrink to expand

    for_subdirs.css_float(css::fl_left);
    for_subdirs.css_float_clear(css::fc_both);
    for_subdirs.css_width("100%", false);
    for_subdirs.css_border_width(css::bd_left, css::bd_thin);
    for_subdirs.css_border_style(css::bd_left, css::bd_solid);
    for_subdirs.css_border_color(css::bd_left, COLOR_MENU_BORDER_OFF);
    for_subdirs.css_margin_left("1em");

	// to have the name of the directory beside the shrink/expand button
    nosubdir.set_no_CR();
    shrink.set_no_CR();
    expand.set_no_CR();
    name.set_no_CR();

	// The body_builder tree
    adopt(&nosubdir);
    adopt(&shrink);
    adopt(&expand);
    adopt(&name);
    adopt(&for_subdirs);

	// binding to events
    shrink.record_actor_on_event(this, event_shrink);
    expand.record_actor_on_event(this, event_expand);

	// set initial visibility of objects
    on_event(event_shrink);
}

void html_dir_tree::clear()
{
    vector<html_dir_tree *>::iterator it = subdirs.begin();

    while(it != subdirs.end())
    {
	if(*it != NULL)
	{
	    delete *it;
	    *it = NULL;
	}
	++it;
    }
    subdirs.clear();
    src = NULL;
    info_read = false;
    has_sub = true;
}

void html_dir_tree::set_source_no_expand(const archive_init_list *ref)
{
    if(ref == NULL)
	throw WEBDAR_BUG;
    src = ref;
    if(!src->has_subdirectory(my_path.display(true)))
	go_hide();
    else
    {
	nosubdir.set_visible(false);
	visibility_has_changed = true;
    }
}

void html_dir_tree::set_source(const archive_init_list *ref)
{
    set_source_no_expand(ref);
    go_expand();
}

void html_dir_tree::set_css_classid(const std::string & classid)
{
    last_classid = classid; // to be able to set future subdirectories
    shrink.set_url_classid(classid);
    expand.set_url_classid(classid);
    name.set_url_classid(classid);
    for(vector<html_dir_tree *>::iterator it = subdirs.begin();
	it != subdirs.end();
	++it)
	if(*it == NULL)
	    throw WEBDAR_BUG;
	else
	    (*it)->set_css_classid(classid);
}

void html_dir_tree::go_shrink()
{
    if(has_sub)
    {
	visibility_has_changed = true;
	shrink.set_visible(false);
	expand.set_visible(true);
	for_subdirs.set_visible(false);
    }
}

void html_dir_tree::go_expand()
{
    if(has_sub)
    {
	visibility_has_changed = true;
	shrink.set_visible(true);
	expand.set_visible(false);
	if(!info_read)
	    go_init_indent();
	if(subdirs.size() > 0)
	    for_subdirs.set_visible(true);
    }
}

void html_dir_tree::go_hide()
{
    has_sub = false;
    shrink.set_visible(false);
    expand.set_visible(false);
    nosubdir.set_visible(true);
    for_subdirs.set_visible(false);
    visibility_has_changed = true;
}

void html_dir_tree::on_event(const std::string & event_name)
{
    if(event_name == event_shrink)
	go_shrink();
    else if(event_name == event_expand)
	go_expand();
    else if(event_name == event_click)
	; // for now do nothing
    else
	throw WEBDAR_BUG;
}

string html_dir_tree::get_body_part(const chemin & path,
				   const request & req)
{
    string ret;

    do
    {
	visibility_has_changed = false;
	ret = html_div::get_body_part(path, req);
    }
    while(visibility_has_changed);

    return ret;
}


void html_dir_tree::go_init_indent()
{
    if(!info_read)
    {
	info_read = true;

	if(src == NULL)
	    throw WEBDAR_BUG;

	vector<libdar::list_entry> tmp = src->get_children_in_table(my_path.display(true));
	vector<libdar::list_entry>::const_iterator it = tmp.begin();

	while(it != tmp.end())
	{
	    if(it->is_dir())
	    {
		html_dir_tree *tmp_sub = new (nothrow) html_dir_tree(src, (my_path + chemin(it->get_name())).display(true));

		if(tmp_sub == NULL)
		    throw exception_memory();

		try
		{
		    if(last_classid != "")
			tmp_sub->set_css_classid(last_classid);
		    subdirs.push_back(tmp_sub);
		}
		catch(...)
		{
		    delete tmp_sub;
		    throw;
		}

		for_subdirs.adopt(tmp_sub);
	    }
	    ++it;
	}
    }
}
