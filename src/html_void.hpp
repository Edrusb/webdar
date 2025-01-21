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

#ifndef HTML_VOID_HPP
#define HTML_VOID_HPP

    /// \file defines two classes html_void and html_void_parent_notifier

    /// - html_void is a transparent type of object (it do not generate any HTTP body
    ///   outside what its adopted children produce). It is used to provide a global
    ///   visibility switch to objects they have adopted
    /// - html_void_parent_notifier is an interface. An object adopting an html_void component
    ///   can implement (but is not obliged to) this interface to be notified of status changes of the
    ///   html_void object, by implementing (overriding) the specified methods of that class.

    // C system header files
extern "C"
{

}

    // C++ system header files
#include "body_builder.hpp"

    // webdar headers


    /// to be used *eventually* by class of object adopting an html_void object

    /// if parent object of an html_void implements this interface it will be notified
    /// of the events this html_void hits. Only the object that has adopted a given
    /// html_void is notified of events met that html_void.

class html_void_parent_notifier
{
public:
    html_void_parent_notifier() {}
    html_void_parent_notifier(const html_void_parent_notifier & ref) = default;
    html_void_parent_notifier(html_void_parent_notifier && ref) noexcept = default;
    html_void_parent_notifier & operator = (const html_void_parent_notifier & ref) = default;
    html_void_parent_notifier & operator = (html_void_parent_notifier && ref) noexcept = default;
    virtual ~html_void_parent_notifier() = default;

        /// propagates body_builder::my_visibility_has_changed()

        /// \param[in] void is the html_void child object that triggered this call
    virtual void void_child_visibility_has_changed(body_builder* voidobj) {};

        /// propagates body_builder::has_adopted()

        /// \param[in] void is the html_void child object that triggered this call
        /// \param[in] obj is the html_void's child that has been adopted
    virtual void void_child_has_adopted(body_builder* voidobj, body_builder* obj) {};

        /// propagates body_builder::will_foresake()

        /// \param[in] void is the html_void child object that triggered this call
        /// \param[in] obj is the html_void's child that is about to be foresaken
    virtual void void_child_will_foresake(body_builder* voidobj, body_builder* obj) {};

};

    /// class html_void is only used to provide global visibility switch to all its adopted children

class html_void: public body_builder
{
public:
    html_void() {}
    html_void(const html_void & ref) = default;
    html_void(html_void && ref) noexcept = default;
    html_void & operator = (const html_void & ref) = default;
    html_void & operator = (html_void && ref) noexcept = default;
    ~html_void() = default;

protected:
        /// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
                                                const request & req) override;

        /// inherited from body_builder
    virtual void my_visibility_has_changed() override;

        /// inherited from body_builder
    virtual void has_adopted(body_builder *obj) override;

        /// inherited from body_builder
    virtual void will_foresake(body_builder *obj) override;

private:
    html_void_parent_notifier* parent_notifier();

};


#endif
