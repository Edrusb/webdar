    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_crypto_algo.hpp"

using namespace std;

string html_crypto_algo::changed = "html_crypto_algo_changed";

html_crypto_algo::html_crypto_algo(const string & title): html_form_select(title)
{
    label.push_back("none");
    label.push_back("scram");
    label.push_back("blowfish");
    label.push_back("aes");
    label.push_back("twofish");
    label.push_back("serpent");
    label.push_back("camellia");
    add_choice(label[0], "None");
    add_choice(label[1], "Scrambling (weak)");
    add_choice(label[2], "Blowfish");
    add_choice(label[3], "AES");
    add_choice(label[4], "Twofish");
    add_choice(label[5], "Serpent");
    add_choice(label[6], "Camellia");
    set_selected(0);
    register_name(changed);
}

libdar::crypto_algo html_crypto_algo::get_value() const
{
    switch(get_selected_num())
    {
    case 0:
	return libdar::crypto_none;
    case 1:
	return libdar::crypto_scrambling;
    case 2:
	return libdar::crypto_blowfish;
    case 3:
	return libdar::crypto_aes256;
    case 4:
	return libdar::crypto_twofish256;
    case 5:
	return libdar::crypto_serpent256;
    case 6:
	return libdar::crypto_camellia256;
    default:
	throw WEBDAR_BUG;
    }
}

void html_crypto_algo::set_value(libdar::crypto_algo val)
{
    switch(val)
    {
    case libdar::crypto_none:
	set_selected(0);
	break;
    case libdar::crypto_scrambling:
	set_selected(1);
	break;
    case libdar::crypto_blowfish:
	set_selected(2);
	break;
    case libdar::crypto_aes256:
	set_selected(3);
	break;
    case libdar::crypto_twofish256:
	set_selected(4);
	break;
    case libdar::crypto_serpent256:
	set_selected(5);
	break;
    case libdar::crypto_camellia256:
	set_selected(6);
	break;
    default:
	throw WEBDAR_BUG;
    }
}
