/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2024 Denis Corbin
//
// This file is part of Webdar
//
//  Webdar is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Webdar is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
//
//----
//  to contact the author: dar.linux@free.fr
/*********************************************************************/

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/tools.hpp>

    // webdar headers
#include "exceptions.hpp"
#include "webdar_css_style.hpp"

    //
#include "html_version.hpp"

#define YES_NO(x) (x ? gettext("YES") : gettext("NO"))

using namespace std;

html_version::html_version():
    html_popup(90, 90),
    webdar_table(2),
    libthreadar_table(2),
    libdar_table(2),
    close("Close", event_close)
{

	// component configuration

    title.clear();
    title.add_text(3, "Webdar version information");

    webdar_table.css_border_collapsed(true);
    libthreadar_table.css_border_collapsed(true);
    libdar_table.css_border_collapsed(true);

    webdar_table.clear();
    webdar_table.adopt_static_html("Webdar info");
    webdar_table.adopt_static_html("");
    webdar_table.adopt_static_html("Version");
    webdar_table.adopt_static_html(WEBDAR_VERSION);

    fill_libthreadar(libthreadar_table);
    fill_libdar(libdar_table);

	// adoption tree

    title_box.adopt(&title);
    adopt(&title_box);

    adopt(&webdar_table);
    adopt(&libthreadar_table);
    adopt(&libdar_table);

    adopt(&close);

	// events

    close.record_actor_on_event(this, event_close);

	// css

    title.add_css_class(webdar_css_style::text_shadow_dark);
    title.add_css_class(webdar_css_style::text_top_center);

    webdar_table.add_css_class(css_table);
    libthreadar_table.add_css_class(css_table);
    libdar_table.add_css_class(css_table);

    webdar_table.set_css_class_cells(css_table_cells);
    libthreadar_table.set_css_class_cells(css_table_cells);
    libdar_table.set_css_class_cells(css_table_cells);

    webdar_table.set_css_class_first_row(css_table_top);
    libthreadar_table.set_css_class_first_row(css_table_top);
    libdar_table.set_css_class_first_row(css_table_top);

    webdar_table.set_css_class_first_column(css_table_left);
    libthreadar_table.set_css_class_first_column(css_table_left);
    libdar_table.set_css_class_first_column(css_table_left);

    webdar_css_style::normal_button(close);
    close.add_css_class(css_close);
}


void html_version::on_event(const string & event_name)
{
    if(event_name == event_close)
	set_visible(false);
    else
	throw WEBDAR_BUG;
}

void html_version::new_css_library_available()
{
    css_class page("html_summary_page");
    css tmp;
    unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    if(!csslib->class_exists(css_close))
    {
	tmp.clear();
	tmp.css_position_type(css::pos_sticky);
	tmp.css_float(css::fl_right);
	tmp.css_position_bottom("1em");
	tmp.css_position_right("1em");
	tmp.css_margin_top("1em");
	csslib->add(css_close, tmp);
    }

    if(!csslib->class_exists(css_table))
    {
	tmp.clear();
	tmp.css_width("90%", true);
	tmp.css_font_size("0.8em");
	tmp.css_margin_top("1em");
	csslib->add(css_table, tmp);
    }

    if(!csslib->class_exists(css_table_top))
    {
	tmp = webdar_css_style::get_css_class(webdar_css_style::text_shadow_dark).get_value();

	tmp.css_border_width(css::bd_all, css::bd_thin);
	tmp.css_border_style(css::bd_all, css::bd_solid);
	tmp.css_font_weight_bold();
	tmp.css_background_color(COLOR_BACK);
	tmp.css_color(COLOR_TEXT);
	csslib->add(css_table_top, tmp);
    }

    if(!csslib->class_exists(css_table_left))
    {
	tmp.clear();
	tmp.css_border_width(css::bd_all, css::bd_thin);
	tmp.css_border_style(css::bd_all, css::bd_solid);
	tmp.css_text_h_align(css::al_left);
	tmp.css_font_weight_bold();
	tmp.css_max_width("30%");
	csslib->add(css_table_left, tmp);
    }

    if(!csslib->class_exists(css_table_cells))
    {
	tmp.clear();
	tmp.css_border_width(css::bd_all, css::bd_thin);
	tmp.css_border_style(css::bd_all, css::bd_solid);
	tmp.css_text_h_align(css::al_center);
	tmp.css_width("90%", true);
	tmp.css_font_size("0.8em");
	tmp.css_margin_top("1em");
	csslib->add(css_table_cells, tmp);
    }

    webdar_css_style::update_library(*csslib);
}

void html_version::fill_libthreadar(html_table & table)
{
    unsigned int maj, med, min;
    string bar_impl;

    libthreadar::get_version(maj, med, min);
    bar_impl = libthreadar::barrier::used_implementation();

    table.adopt_static_html("Libthreader info");
    table.adopt_static_html("");
    table.adopt_static_html("Libthreadar version");
    table.adopt_static_html(libdar::tools_printf("%d.%d.%d", maj, med, min));
    table.adopt_static_html("Barrier implementation");
    table.adopt_static_html(bar_impl);
}

void html_version::fill_libdar(html_table & table)
{
    libdar::U_I maj, med, min;
    const char *endy = nullptr;
    string time_accuracy;

    libdar::get_version(maj, med, min);

    table.adopt_static_html("Libdar info");
    table.adopt_static_html("");
    table.adopt_static_html("Libdar version");
    table.adopt_static_html(libdar::tools_printf("%d.%d.%d", maj, med, min));
    table.adopt_static_html("");
    table.adopt_static_html("");

    table.adopt_static_html("gzip compression (libz)");       table.adopt_static_html(YES_NO(libdar::compile_time::libz()));
    table.adopt_static_html("bzip2 compression (libbzip2)");  table.adopt_static_html(YES_NO(libdar::compile_time::libbz2()));
    table.adopt_static_html("lzo compression (liblzo2)");     table.adopt_static_html(YES_NO(libdar::compile_time::liblzo()));
    table.adopt_static_html("xz compression (liblzma)");      table.adopt_static_html(YES_NO(libdar::compile_time::libxz()));
    table.adopt_static_html("zstd compression (libzstd)");    table.adopt_static_html(YES_NO(libdar::compile_time::libzstd()));
    table.adopt_static_html("lz4 compression (liblz4)");      table.adopt_static_html(YES_NO(libdar::compile_time::liblz4()));
    table.adopt_static_html("Strong encryption (libgcrypt)"); table.adopt_static_html(YES_NO(libdar::compile_time::libgcrypt()));
    table.adopt_static_html("Public key ciphers (gpgme)");    table.adopt_static_html(YES_NO(libdar::compile_time::public_key_cipher()));
    table.adopt_static_html("Extended Attributes support");   table.adopt_static_html(YES_NO(libdar::compile_time::ea()));
    table.adopt_static_html("Large files support (> 2GB)");   table.adopt_static_html(YES_NO(libdar::compile_time::largefile()));
    table.adopt_static_html("ext2fs NODUMP flag support");    table.adopt_static_html(YES_NO(libdar::compile_time::nodump()));

    table.adopt_static_html("Integer size used");
    if(libdar::compile_time::bits() == 0)
	table.adopt_static_html("unlimited");
    else
	table.adopt_static_html(libdar::tools_printf("%d bits", libdar::compile_time::bits()));

    table.adopt_static_html("Thread safe support");          table.adopt_static_html(YES_NO(libdar::compile_time::thread_safe()));
    table.adopt_static_html("Furtive read mode support");    table.adopt_static_html(YES_NO(libdar::compile_time::furtive_read()));
    table.adopt_static_html("Linux ext2/3/4 FSA support");   table.adopt_static_html(YES_NO(libdar::compile_time::FSA_linux_extX()));
    table.adopt_static_html("Mac OS X HFS+ FSA support");    table.adopt_static_html(YES_NO(libdar::compile_time::FSA_birthtime()));
    table.adopt_static_html("Linux statx() support");        table.adopt_static_html(YES_NO(libdar::compile_time::Linux_statx()));


    switch(libdar::compile_time::system_endian())
    {
    case libdar::compile_time::big:
	endy = "big";
	break;
    case libdar::compile_time::little:
	endy = "little";
	break;
    case libdar::compile_time::error:
	endy = "error!";
	break;
    default:
	endy = "unknown!!!";
	break;
    }
    table.adopt_static_html("Detected system/CPU endian");   table.adopt_static_html(endy);

    table.adopt_static_html("Posix fadvise support");        table.adopt_static_html(YES_NO(libdar::compile_time::posix_fadvise()));
    table.adopt_static_html("Large dir speed optimization"); table.adopt_static_html(YES_NO(libdar::compile_time::fast_dir()));

    if(libdar::compile_time::nanosecond_read())
	time_accuracy = "1 nanosecond";
    else
    {
	if(libdar::compile_time::microsecond_read())
	    time_accuracy = "1 microsecond";
	else
	    time_accuracy = "1 second";
    }

    table.adopt_static_html("Timestamp read accuracy");      table.adopt_static_html(time_accuracy);

    if(libdar::compile_time::nanosecond_write())
	time_accuracy = "1 nanosecond";
    else
    {
	if(libdar::compile_time::microsecond_write())
	    time_accuracy = "1 microsecond";
	else
	    time_accuracy = "1 second";
    }

    table.adopt_static_html("Timestamp write accuracy");     table.adopt_static_html(time_accuracy);
    table.adopt_static_html("Restores dates of symlinks");   table.adopt_static_html(YES_NO(libdar::compile_time::symlink_restore_dates()));

    table.adopt_static_html("Multiple threads support");     table.adopt_static_html(YES_NO(libdar::compile_time::libthreadar()));
    if(libdar::compile_time::libthreadar())
    {
	table.adopt_static_html("libthreadar version");
	table.adopt_static_html(libdar::compile_time::libthreadar_version());
    }

    table.adopt_static_html("Delta compression");            table.adopt_static_html(YES_NO(libdar::compile_time::librsync()));
    table.adopt_static_html("Remote repository");            table.adopt_static_html(YES_NO(libdar::compile_time::remote_repository()));
    if(libdar::compile_time::remote_repository())
    {
	table.adopt_static_html("libcurl version");
	table.adopt_static_html(libdar::compile_time::libcurl_version());
    }

    table.adopt_static_html("argon2 hashing");               table.adopt_static_html(YES_NO(libdar::compile_time::libargon2()));
    table.adopt_static_html("Whirlpool hashing");            table.adopt_static_html(YES_NO(libdar::compile_time::whirlpool_hash()));
}
