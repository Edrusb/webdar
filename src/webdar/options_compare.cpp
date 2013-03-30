    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "webdar_tools.hpp"


    //
#include "options_compare.hpp"

using namespace std;

options_compare::options_compare():
    visible(true),
    form("Update options"),
    fs("Comparison options"),
    info_details("Detailed information",
		 html_form_input::check,
		 "",
		 1),
    alter_atime("Modify atime",
		html_form_input::check,
		"",
		1),
    furtive_read_mode("Furtive read mode",
		      html_form_input::check,
		      "",
		      1),
    display_skipped("Display skipped files",
		    html_form_input::check,
		    "",
		    1),
    hourshift("Ignore difference of exactly +/- N hour(s)",
	      html_form_input::number,
	      "0",
	      2),
    compare_symlink_date("Compare symlink mtime",
			 html_form_input::check,
			 "",
			 1)
{
    libdar::archive_options_diff defaults;

	/// default values
    info_details.set_value_as_bool(defaults.get_info_details());
    what_to_check.set_value(defaults.get_what_to_check());
    alter_atime.set_value_as_bool(defaults.get_alter_atime());
    furtive_read_mode.set_value_as_bool(defaults.get_furtive_read_mode());
    display_skipped.set_value_as_bool(defaults.get_display_skipped());
    hourshift.set_value(webdar_tools_convert_to_string(defaults.get_hourshift()));
    compare_symlink_date.set_value_as_bool(defaults.get_compare_symlink_date());

	/// building HTML dependancies

    fs.adopt(&info_details);
    fs.adopt(&what_to_check);
    fs.adopt(&alter_atime);
    fs.adopt(&furtive_read_mode);
    fs.adopt(&display_skipped);
    fs.adopt(&hourshift);
    fs.adopt(&compare_symlink_date);
    form.adopt(&fs);
    adopt(&form);
}

libdar::archive_options_diff options_compare::get_options() const
{
    libdar::archive_options_diff ret;

    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_what_to_check(what_to_check.get_value());
    ret.set_alter_atime(alter_atime.get_value_as_bool());
    ret.set_furtive_read_mode(furtive_read_mode.get_value_as_bool());
    ret.set_display_skipped(display_skipped.get_value_as_bool());
    ret.set_hourshift(libdar::infinint(webdar_tools_convert_to_int(hourshift.get_value())));
    ret.set_compare_symlink_date(compare_symlink_date.get_value_as_bool());

    return ret;
}


std::string options_compare::get_body_part(const chemin & path,
					   const request & req)
{
    if(visible)
	return get_body_part_from_all_children(path, req);
    else
	return "";
}

