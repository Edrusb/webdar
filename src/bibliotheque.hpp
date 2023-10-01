/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
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

#ifndef BIBLIOTHEQUE_HPP
#define BIBLIOTHEQUE_HPP

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

    /// \file bibliotheque.hpp defines bibliotheque class
    ///
    /// class blibliotheque purpose is to assign a label per category
    /// to jsoner objects that constitue a given configuration set.
    /// Being itself a jsoner this object is in charge of the global
    /// format of the configuration file, calling in turn the different
    ///

class bibliotheque : public jsoner
{
public:
	/// json objects are split into category, each used as a different namespace
    enum category { filter, command, repo, confsave, conftest, confdiff, conflist, confrest, confmerg, confrepair, confcommon };

    bibliotheque() = default;
    bibliotheque(const bibliotheque & ref) = default;
    bibliotheque(bibliotheque && ref) noexcept(false) = default;
    bibliotheque & operator = (const bibliotheque & ref) = default;
    bibliotheque & operator = (bibliotheque && ref) noexcept(false) = default;
    virtual ~bibliotheque() {};

    void set_config_filename();
    void get_config_filename();
    virtual void read_json(const libdar::fichier_local & ref) override;
    virtual void write_json(const libdar::fichier_local & ref) override;

	////// attention, les objets references ne sont copiables etc. seulement s'ils sont sans reference !!!
	////// ca veut dire que s'ils sont crees en dehors de Bibiotheque et y sont ajoutes apres
	////// il faut les fournir par leur reference/adress/unique_ptr
        //////
	////// corolaire le remplacement d'un objet par un autre
        //////
	////// le reference d'un objet par une autre lors de sa creation se fait par son nom au sein de la
	////// bibiotheque, le parent doit aller chercher l'objet et l'adopter. Ainsi un enfant peut avoir plusieurs parents /!\

    void add(category cat, const std::string & name, std::shared_ptr<jsoner> obj);
    void replace(category cat, const std::string & name, std::shared_ptr<jsoner> obj);
    void delete(category cat, const std::string & name);

    void reset_read(category cat);
    bool read_next(category cat, std::string & name);

	/// only child are available for lookup per name
	/// target of this call is for a json_parent to register a child given its name
    bool find_by_name(category cat, std::string & name, json_child* found);

	/// provides a reference to an existing object of the bibliotheque
    std::unique_ptr<bibli_renvoi> create_renvoi(category cat, std::string & name);

private:
    std::map<std::string, bibli_referable> filters;
    std::map<std::string, bibli_referable> command;
    std::map<std::string, bibli_referable> repo;
    std::map<std::string, bibli_referable> save;
    std::map<std::string, bibli_referable> test;
    std::map<std::string, bibli_referable> diff;
    std::map<std::string, bibli_referable> list;
    std::map<std::string, bibli_referable> rest;
    std::map<std::string, bibli_referable> merg;
    std::map<std::string, bibli_referable> repair;
    std::map<std::string, bibli_referable> common; /// object of that kind are not libdarer, they are referred by action relative configurations

    mutable std::map<std::string, bibli_referable>::iterator read_filters;
    mutable std::map<std::string, bibli_referable>::iterator read_command;
    mutable std::map<std::string, bibli_referable>::iterator read_repo;
    mutable std::map<std::string, bibli_referable>::iterator read_save;
    mutable std::map<std::string, bibli_referable>::iterator read_test;
    mutable std::map<std::string, bibli_referable>::iterator read_diff;
    mutable std::map<std::string, bibli_referable>::iterator read_list;
    mutable std::map<std::string, bibli_referable>::iterator read_rest;
    mutable std::map<std::string, bibli_referable>::iterator read_repair;
    mutable std::map<std::string, bibli_referable>::iterator read_common;

};

#endif
