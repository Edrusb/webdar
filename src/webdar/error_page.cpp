    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_page.hpp"
#include "html_text.hpp"
#include "html_url.hpp"
#include "html_table.hpp"


    //
#include "error_page.hpp"

using namespace std;


answer error_page::give_answer(const request & req)
{
    answer ret;
    html_page page = html_page(msg);
    html_table table = 1;
    html_text text = 1;

    text.add_text(true, false, false, html_url("/", body).display());
    table.set_border(5);
    table.add_cell(text.display());
    text.clear(0);
    text.add_text(false, false, false, "Requested ressource was:");
    text.add_nl();
    text.add_text(false, false, true, req.get_uri().get_string());
    table.add_cell(text.display());
    page.add_to_body(table.display());

    ret.set_status(status);
    ret.set_reason(msg);
    ret.add_body(page.display());
    return ret;
}

