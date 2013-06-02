    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers

    //
#include "html_libdar_running.hpp"

using namespace std;

libdar_running::libdar_running():
    html_page("Libdar thread current status"),
    ask_close("Stop libdar thread", ask_end_libdar),
    force_close("Abord libdar thread", force_end_libdar),
    finish("Finish", close_libdar_screen)
{
    adopt(&web_ui);
    adopt(&stats);
    adopt(&ask_close);
    adopt(&force_close);
    adopt(&finish);
}

