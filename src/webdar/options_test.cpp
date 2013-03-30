    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "options_test.hpp"

using namespace std;

options_test::options_test():
    form("Update options"),
    fs("Testing options"),
    info_details("Detailed informations",
		 html_form_input::check,
		 "",
		 1),
    empty("Dry-run execution",
	  html_form_input::check,
	  "",
	  1),
    display_skipped("Display skipped files",
		    html_form_input::check,
		    "",
		    1)
{
    libdar::archive_options_test defaults;

	// default values
    info_details.set_value_as_bool(defaults.get_info_details());
    empty.set_value_as_bool(defaults.get_empty());
    display_skipped.set_value_as_bool(defaults.get_display_skipped());

	// building HTML structure

    fs.adopt(&info_details);
    fs.adopt(&empty);
    fs.adopt(&display_skipped);
    form.adopt(&fs);
    adopt(&form);
}

libdar::archive_options_test options_test::get_options() const
{
    libdar::archive_options_test ret;

    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_empty(empty.get_value_as_bool());
    ret.set_display_skipped(display_skipped.get_value_as_bool());

    return ret;
}

string options_test::get_body_part(const chemin & path,
				   const request & req)
{
    return get_body_part_from_all_children(path, req);
}
