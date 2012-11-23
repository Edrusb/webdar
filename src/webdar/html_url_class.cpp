    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_url_class.hpp"

using namespace std;


string html_url_class::get_body_part(const chemin & path,
				     const request & req)
{
    string class_name = get_path().namify();
    string ret = "";

    if(link != "")
	ret += "a." + class_name + ":link {" + link + "}\n";
    if(visited != "")
	ret += "a." + class_name + ":visited {" + visited + "}\n";
    if(active != "")
	ret += "a." + class_name + ":active {" + active + "}\n";
    if(hover != "")
	ret += "a." + class_name + ":hover {" + hover + "}\n";

    if(ret != "")
	ret = "<style>\n" + ret + "</style>\n";

    return ret;
}

