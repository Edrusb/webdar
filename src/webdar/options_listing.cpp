    // C system header files
extern "C"
{

}

    // C++ system header files
#include <new>

    // webdar headers
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_div.hpp"
#include "exceptions.hpp"

    //
#include "options_listing.hpp"

using namespace std;

options_listing::options_listing():
    form("Update"),
    fs("Listing options"),
    info_details("Detailed informations",
		 html_form_input::check,
		 "",
		 0),
    filter_unsaved("Filter out unsaved files",
		   html_form_input::check,
		   "",
		   0),
    display_ea("Display Extended Attributes",
	       html_form_input::check,
	       "",
	       0)
{
    fs.adopt(&info_details);
    fs.adopt(&filter_unsaved);
    fs.adopt(&display_ea);
    form.adopt(&fs);
    div.adopt(&form);
    adopt(&div);
}

string options_listing::get_body_part(const chemin & path,
				       const request & req)
{
    return get_body_part_from_all_children(path, req);
}


libdar::archive_options_listing options_listing::get_options() const
{
    libdar::archive_options_listing ret;

    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_filter_unsaved(filter_unsaved.get_value_as_bool());
    ret.set_display_ea(display_ea.get_value_as_bool());

    return ret;
}
