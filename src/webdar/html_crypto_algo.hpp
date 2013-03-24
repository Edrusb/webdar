#ifndef HTML_CRYPTO_ALGO_HPP
#define HTML_CRYPTO_ALGO_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <vector>
#include <string>
#include <dar/libdar.hpp>

    // webdar headers
#include "html_form_select.hpp"
#include "body_builder.hpp"

class html_crypto_algo : public html_form_select
{
public:
    static std::string changed;

    html_crypto_algo(const std::string & title);

    libdar::crypto_algo get_value() const;
    void set_value(libdar::crypto_algo val);

private:
    std::vector< std::string > label;
};

#endif
