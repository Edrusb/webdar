    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_statistics.hpp"

using namespace std;

html_statistics::html_statistics()
{
    table = NULL;

    treated_lbl.css_text_align(al_right);
    hard_links_lbl.css_text_align(al_right);
    skipped_lbl.css_text_align(al_right);
    ignored_lbl.css_text_align(al_right);
    tooold_lbl.css_text_align(al_right);
    errored_lbl.css_text_align(al_right);
    deleted_lbl.css_text_align(al_right);
    ea_deleted_lbl.css_text_align(al_right);
    byte_amount_lbl.css_text_align(al_right);

    treated_count.css_font_weight_bold();
    hard_links_count.css_font_weight_bold();
    skipped_count.css_font_weight_bold();
    ignored_count.css_font_weight_bold();
    tooold_count.css_font_weight_bold();
    errored_count.css_font_weight_bold();
    deleted_count.css_font_weight_bold();
    ea_deleted_count.css_font_weight_bold();
    byte_amount_count.css_font_weight_bold();
}

void html_statistics::clear_labels()
{
    treated_lbl.clear();
    hard_links_lbl.clear();
    skipped_lbl.clear();
    ignored_lbl.clear();
    tooold_lbl.clear();
    errored_lbl.clear();
    deleted_lbl.clear();
    ea_deleted_lbl.clear();
    byte_amount_lbl.clear();
}

void html_statistics::build()
{
    if(table != NULL)
	throw WEBDAR_BUG; // table is already built
    table = new (nothrow) html_table(2);
    if(table == NULL)
	throw WEBDAR_BUG;
    try
    {
	if(treated_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(treated_lbl.get_body_part());
	    table->adopt(&treated_count);
	}
	if(hard_links_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(hard_links_lbl.get_body_part());
	    table->adopt(&hard_links_count);
	}
	if(skipped_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(skipped_lbl.get_body_part());
	    table->adopt(&skipped_count);
	}
	if(ignored_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(ignored_lbl.get_body_part());
	    table->adopt(&ignored_count);
	}
	if(tooold_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(tooold_lbl.get_body_part());
	    table->adopt(&tooold_count);
	}
	if(errored_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(errored_lbl.get_body_part());
	    table->adopt(&errored_count);
	}
	if(deleted_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(deleted_lbl.get_body_part());
	    table->adopt(&deleted_count);
	}
	if(ea_deleted_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(ea_deleted_lbl.get_body_part());
	    table->adopt(&ea_deleted_count);
	}
	if(byte_amount_lbl.get_body_part() != "")
	{
	    table->adopt_static_html(byte_amount_lbl.get_body_part());
	    table->adopt(&byte_amount_count);
	}
    }
    catch(...)
    {
	if(table != NULL)
	{
	    delete table;
	    table = NULL;
	}
	throw;
    }
}

void html_statistics::unbuild()
{
    if(table == NULL)
	throw WEBDAR_BUG; // table not built cannot "unbuild" it

    delete table;
    table = NULL;
}
