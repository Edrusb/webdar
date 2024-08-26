/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2024 Denis Corbin
//
// This file is part of Webdar
//
//  Webdar is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Webdar is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
//
//----
//  to contact the author: dar.linux@free.fr
/*********************************************************************/

#ifndef HTML_BIBLIOTHEQUE_HPP
#define HTML_BIBLIOTHEQUE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <map>
#include <string>
#include <memory>

    // webdar headers
#include "jsoner.hpp"
#include "bibli_renvoi.hpp"
#include "bibli_referable.hpp"
#include "body_builder.hpp"
#include "html_text.hpp"
#include "html_tabs.hpp"

    /// \file html_bibliotheque.hpp defines html_bibliotheque class


    /// class html_bibliotheque

    /// class html_bibliotheque purpose is to assign a label per category
    /// to jsoner objects that constitue a given configuration set.
    /// Being itself a jsoner this object is in charge of the global
    /// format of the configuration file, calling in turn the different

class html_bibliotheque : public jsoner, public body_builder
{
public:
	/// json objects are split into category, each used as a different namespace
    enum category { filefilter, pathfilter, command, repo, confsave, conftest, confdiff, conflist, confrest, confmerg, confrepair, confcommon, EOE };
    void begin(category & cat); // reset to first value
    bool end(category cat) { return cat >= EOE; };
    void incr(category & v); // ++cat implementation with out of bound control

    html_bibliotheque();
    html_bibliotheque(const html_bibliotheque & ref) = delete;
    html_bibliotheque(html_bibliotheque && ref) noexcept(false) = delete;
    html_bibliotheque & operator = (const html_bibliotheque & ref) = delete;
    html_bibliotheque & operator = (html_bibliotheque && ref) noexcept(false) = delete;
    virtual ~html_bibliotheque() = default;

	////// attention, les objets references ne sont copiables etc. seulement s'ils sont sans reference !!!
	////// ca veut dire que s'ils sont crees en dehors de Bibiotheque et y sont ajoutes apres
	////// il faut les fournir par leur reference/adress/unique_ptr
        //////
	////// corolaire le remplacement d'un objet par un autre
        //////
	////// le reference d'un objet par une autre lors de sa creation se fait par son nom au sein de la
	////// bibiotheque, le parent doit aller chercher l'objet et l'adopter. Ainsi un enfant peut avoir plusieurs parents [!]

    void add(category cat, const std::string & name, const  bibli_referable & obj, bool can_replace = false);
    bool remove(category cat, const std::string & name); ///< \returns true if an object was found and deleted

    void reset_read(category cat) const;
    bool read_next(category cat, std::string & name) const;

	/// only child are available for lookup per name
	/// target of this call is for a json_parent to register a child given its name
    bool find_by_name(category cat, std::string & name, bibli_referable & found) const;

    virtual void read_json(const libdar::fichier_global & ref) override {};
    virtual void write_json(const libdar::fichier_global & ref) const override {};


protected:
	// from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

    virtual void new_css_library_available() override;

private:

	/// structure used to record components stored in the bibliotheque
    struct referable_list
    {
	std::map<std::string, bibli_referable> refs;
	mutable std::map<std::string, bibli_referable>::const_iterator read_index;

	referable_list() { refs.clear(); reset_read(); };
	void reset_read() const { read_index = refs.begin(); };
    };

    typedef std::map<category, referable_list>::iterator content_index;
    typedef std::map<category, referable_list>::const_iterator const_content_index;
    typedef std::map<std::string, bibli_referable>::iterator refs_index;
    typedef std::map<std::string, bibli_referable>::const_iterator const_refs_index;

    std::map<category, referable_list> content;

    html_tabs tabs;
    static constexpr unsigned int numtabs = EOE; // EOE for End Of Enumeration
    html_text text[numtabs];

    void initialize_content_and_indexes();
    void reset_read_iterators();

    static const char* css_tabs;
};

#endif
