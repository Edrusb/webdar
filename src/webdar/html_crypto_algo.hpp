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

class html_crypto_algo : public body_builder
{
public:
    html_crypto_algo();

    libdar::crypto_algo get_value() const;
    void set_value(libdar::crypto_algo val);

	/// inherited from body_builder
    std::string get_body_part(const chemin & path,
			      const request & req)
    { return sel.get_body_part(path, req); };

private:
    html_form_select sel;
    std::vector<std::string> label;
};

#endif
