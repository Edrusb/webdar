    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_fieldset.hpp"

    //
#include "html_yes_no_box.hpp"


using namespace std;

html_yes_no_box::html_yes_no_box(const std::string & message, bool default_value)
    : form("Confirm"),
      form_fs(message)
{
    rd.add_choice("no", "No");
    rd.add_choice("yes", "Yes");
    rd.set_selected(default_value ? 1 : 0);

	/// building the body_builder tree
    form_fs.adopt(&rd);
    form.adopt(&form_fs);
    adopt(&form);
}
