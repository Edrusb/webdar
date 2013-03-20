    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_text.hpp"
#include "tokens.hpp"
#include "exceptions.hpp"

    //
#include "menu.hpp"

using namespace std;

const string menu::changed = "menu_changed";

menu::menu()
{
    css tmp_set;

    current_mode = 0;

	// Common aspects
    box_off.css_border_style(css::bd_all, css::bd_solid, true);
    box_off.css_border_width(css::bd_all, css::bd_medium, true);
    box_off.css_width("8em", true, true);
    box_off.css_padding("0.5em", true);
    box_off.css_margin("0.2em", true);
    box_off.css_text_align(al_center, true);

	// copy common aspects to box_off and box_void
    box_on.css_inherit_from(box_off);
    box_void.css_inherit_from(box_off);
    box_void.css_border_style(css::bd_all, css::bd_none);

	// box_off and tmp_norm COLORS
    tmp_set.css_clear_attributes();
    tmp_set.css_color(COLOR_MENU_FRONT_OFF, true);
    tmp_set.css_background_color(COLOR_MENU_BACK_OFF, true);
    tmp_set.css_font_weight_bold(true);
    tmp_set.css_font_style_italic(true);
    tmp_set.css_text_decoration(css::dc_none, true);
    box_off.css_inherit_from(tmp_set);
    url_normal.set_style_link(tmp_set);
    url_normal.set_style_visited(tmp_set);
    box_off.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF, true);

	// Link Hover and Active in box_off
    tmp_set.css_color(COLOR_MENU_FRONT_HOVER_OFF, true);
    tmp_set.css_text_decoration(dc_underline, true);
    url_normal.set_style_hover(tmp_set);
    tmp_set.css_color(COLOR_MENU_FRONT_ACTIVE_OFF, true);
    url_normal.set_style_active(tmp_set);

	// box_on and tmp_select COLORS
    tmp_set.css_color(COLOR_MENU_FRONT_ON, true);
    tmp_set.css_background_color(COLOR_MENU_BACK_ON, true);
    tmp_set.css_font_weight_bold(true);
    tmp_set.css_font_style_normal(true);
    tmp_set.css_text_decoration(css::dc_none, true);
    box_on.css_inherit_from(tmp_set);
    url_selected.set_style_link(tmp_set);
    url_selected.set_style_visited(tmp_set);
    box_on.css_border_color(css::bd_all, COLOR_MENU_BORDER_ON, true);

	// Link Hover and Active in box_on
    tmp_set.css_color(COLOR_MENU_FRONT_HOVER_ON, true);
    tmp_set.css_text_decoration(dc_underline, true);
    url_selected.set_style_hover(tmp_set);
    tmp_set.css_color(COLOR_MENU_FRONT_ACTIVE_ON, true);
    url_selected.set_style_active(tmp_set);

    tmp_set.css_clear_attributes();

    adopt(&global);
    adopt(&url_normal);
    adopt(&url_selected);

    register_name(changed); // add the "menu_changed" event to this object
}

menu::~menu()
{
    vector<boite *>::iterator it = item.begin();

    while(it != item.end())
    {
	if(*it != NULL)
	    delete (*it);
	++it;
    }
    item.clear();
}

void menu::add_entry(const std::string & reference, const std::string & label)
{
    chemin tmp = get_path();
    unsigned int i = 0;
    unsigned int s = item.size();
    boite *box = NULL;

    while(i < s && item[i] != NULL && item[i]->value != reference)
	++i;

    if(i < s)
	throw WEBDAR_BUG; // duplicated reference

    tmp.push_back(reference);

    box = new (nothrow) boite(tmp.display(), label);
    if(box == NULL)
	throw exception_memory();
    item.push_back(box);

    box->value = reference;
    box->inside.css_border_style(css::bd_all, css::bd_none);
    box->inside.set_class(url_normal.get_class_id());
    if(label != "")
	box->surround.css_inherit_from(box_off);
    else
	box->surround.css_inherit_from(box_void);

	/// building the body_builder tree
    box->surround.adopt(&(box->inside));
    global.adopt(&(box->surround));

    if(item.size() == 1)
	set_mode(0);
}

string menu::get_current_label() const
{
    boite *box = NULL;

    if(current_mode >= item.size())
	throw WEBDAR_BUG;

    box = item[current_mode];
    if(box == NULL)
	throw WEBDAR_BUG;

    return box->value;
}


string menu::get_body_part(const chemin & path,
			   const request & req)
{
	// reading the requested path to determin
	// whether a change of mode is required
    chemin target = req.get_uri().get_path();
    string choice;

    if(!target.empty())
	choice = target.back();
    target.pop_back();

    if(target == get_path() && choice != "")
    {
	    // the requested link is us
	    // except the last part that we tailed out
	    // and which we ignore if empty
	unsigned int i = 0;
	unsigned int size = item.size();
	while(i < size && item[i] != NULL && item[i]->value != choice)
	    ++i;
	if(i < size)
	    set_mode(i);
    }

    return get_body_part_from_all_children(path, req);
}

void menu::path_has_changed()
{
    vector<boite *>::iterator it = item.begin();

    while(it != item.end())
    {
	chemin chem = get_path();

	if(*it == NULL)
	    throw WEBDAR_BUG;
	chem.push_back((*it)->value);
	(*it)->inside.change_url(chem.display(false));
	(*it)->inside.set_class(url_normal.get_class_id());
	++it;
    }

    if(item.size() > 0)
	set_mode(current_mode);
}

void menu::css_updated(bool inherit)
{
    global.css_inherit_from(*this, true, true);
}


void menu::set_mode(unsigned int mode)
{
    unsigned int size = item.size();
    bool has_changed = (mode != current_mode);
    if(mode >= size)
	throw WEBDAR_BUG;

    if(current_mode >= size)
    	throw WEBDAR_BUG;

    if(item[current_mode] == NULL)
	throw WEBDAR_BUG;

    if(item[mode] == NULL)
	throw WEBDAR_BUG;

	/// all is fine, we can go on modifying the objects

    if(item[current_mode]->inside.get_label() == "")
	item[current_mode]->surround.css_inherit_from(box_void, false, true);
    else
    {
	item[current_mode]->surround.css_inherit_from(box_off, false, true);
	item[current_mode]->inside.set_class(url_normal.get_class_id());
    }

    item[mode]->surround.css_inherit_from(box_on, false, true);
    item[mode]->inside.set_class(url_selected.get_class_id());
    current_mode = mode;
    if(has_changed)
	act(changed); // trigger the "changed" event
}
