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

#ifndef html_bibliotheque_HPP
#define html_bibliotheque_HPP

    // C system header files
#include "my_config.h"

extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "body_builder.hpp"
#include "actor.hpp"
#include "events.hpp"
#include "html_tabs.hpp"
#include "html_form_input_file.hpp"
#include "html_double_button.hpp"
#include "html_form_fieldset.hpp"
#include "html_form.hpp"
#include "bibliotheque.hpp"
#include "arriere_boutique.hpp"
#include "html_entrepot.hpp"
#include "html_mask_form_filename.hpp"
#include "html_mask_form_path.hpp"
#include "html_compression_params.hpp"
#include "html_slicing.hpp"
#include "html_ciphering.hpp"
#include "html_form_sig_block_size.hpp"
#include "html_form_overwrite_action.hpp"
#include "html_options_test.hpp"
#include "html_options_compare.hpp"
#include "html_options_read.hpp"
#include "html_options_create.hpp"
#include "html_options_isolate.hpp"
#include "html_options_merge.hpp"
#include "html_options_repair.hpp"
#include "html_options_extract.hpp"

    /// class html_bibliotheque provide graphical mean to manipulate a bibliotheque object

    /// class html_bibliotheque let user (thanks to its web interface) to:
    /// - define, update, remove components to the bibliotheque (per category)
    /// - save/load, upload/download the whole bibliotheque to file or throw the http(s) channel
    ///
    /// \note the downloading process is the following: the download button triggers an event
    /// which drives the html_bibiliotheque to trigger its event_download event. But the download
    /// button has a link that lead the browser to save to file the body of the next http response.
    ///
    /** \verbatim

	+--------------------<top_fs>--------------------------+
	|+---------------------<form><filename_fs>------------+|
	||          +------<filename>--------------------+---+||
	|| filename |                                    |   |||
	||          +------------------------------------+---+||
	||                                                    ||
	|+--<update>------------------------------------------+|
	|                                                      |
	|+------+ +------+                                     |
        || save | | load |                                     |
	|+------+ +------+                                     |
	+------------------------------------------------------+
	+-------------------<bot_fs>---------------------------+
	| +---- -----------+                                   |
	| | file selector  |                                   |
	| +----------------+                                   |
	+------------------------------------------------------+
	| +----------------+                                   |
	| |  Download      |                                   |
	| +----------------+                                   |
	+------------------------------------------------------+
	| +----------------+                                   |
	| | clear configs  |                                   |
	| +----------------+                                   |
	+------------------------------------------------------+

	\endverbatim **/


class html_bibliotheque: public body_builder, public actor, public events
{
public:
    static const std::string event_download;

    html_bibliotheque(std::shared_ptr<bibliotheque> & ptr, const std::string & bib_path);
    html_bibliotheque(const html_bibliotheque & ref) = default;
    html_bibliotheque(html_bibliotheque && ref) noexcept = default;
    html_bibliotheque & operator = (const html_bibliotheque & ref) = default;
    html_bibliotheque & operator = (html_bibliotheque && ref) noexcept = default;
    ~html_bibliotheque() = default;


	/// inherited from body_builder
    virtual void on_event(const std::string & event_name) override;


protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    std::shared_ptr<bibliotheque> biblio;

    std::unique_ptr<arriere_boutique<html_entrepot> > ab_entrepot;
    std::unique_ptr<arriere_boutique<html_mask_form_filename> > ab_filemask;
    std::unique_ptr<arriere_boutique<html_mask_form_path> > ab_pathmask;
    std::unique_ptr<arriere_boutique<html_compression_params> > ab_compr;
    std::unique_ptr<arriere_boutique<html_slicing> > ab_slicing;
    std::unique_ptr<arriere_boutique<html_ciphering> > ab_ciphering;
    std::unique_ptr<arriere_boutique<html_form_sig_block_size> > ab_delta_sig;
    std::unique_ptr<arriere_boutique<html_form_overwrite_action> > ab_over_policy;
    std::unique_ptr<arriere_boutique<html_options_test> > ab_options_test;
    std::unique_ptr<arriere_boutique<html_options_compare> > ab_options_compare;
    std::unique_ptr<arriere_boutique<html_options_read> > ab_options_read;
    std::unique_ptr<arriere_boutique<html_options_create> > ab_options_create;
    std::unique_ptr<arriere_boutique<html_options_isolate> > ab_options_isolate;
    std::unique_ptr<arriere_boutique<html_options_merge> > ab_options_merge;
    std::unique_ptr<arriere_boutique<html_options_repair> > ab_options_repair;
    std::unique_ptr<arriere_boutique<html_options_extract> > ab_options_extract;

    html_tabs tabs;
    html_text intro;
    html_form_fieldset top_fs;
    html_form form;
    html_form_fieldset filename_fs;
    html_form_input_file filename;
    html_form_input autosave;
    html_double_button save;
    html_double_button load;
    html_form_fieldset bot_fs;
    html_form upload_form;
    html_form_input upload_file;
    html_form_fieldset down_fs;
    html_double_button download;
    html_form_fieldset clear_fs;
    html_double_button clear_conf;
    html_double_button generate_defaults;
    html_text ok_loaded;
    html_text ok_saved;
    html_text ok_uploaded;
    html_text ok_cleared;
    html_text nok_message;

    html_div statusbar;
    html_text saved_status;
    html_text unsaved_status;

    bool expect_upload;

    void set_saved_status();
    void clear_ok_messages();
    void set_default_configs();
    void set_default_configs_no_compress(); // called by set_default_configs(), separated method for better readability
    void set_full_from_diff(); // called by set_default_configs(), separated method for better readability

    static constexpr const char* tab_main = "main";
    static constexpr const char* tab_repo = "repo";
    static constexpr const char* tab_filemask = "filemask";
    static constexpr const char* tab_pathmask = "pathmask";
    static constexpr const char* tab_compression = "compression";
    static constexpr const char* tab_slicing = "slicing";
    static constexpr const char* tab_ciphering = "ciphering";
    static constexpr const char* tab_delta_sig = "delta-sig";
    static constexpr const char* tab_over_policy = "over-policy";
    static constexpr const char* tab_options_test = "options-test";
    static constexpr const char* tab_options_compare = "options-compare";
    static constexpr const char* tab_options_read = "options-read";
    static constexpr const char* tab_options_create = "options-create";
    static constexpr const char* tab_options_isolate = "options-isolate";
    static constexpr const char* tab_options_merge = "options-merge";
    static constexpr const char* tab_options_repair = "options-repair";
    static constexpr const char* tab_options_extract = "options-extract";

    static constexpr const char* event_save = "save";
    static constexpr const char* event_load = "load";
    static constexpr const char* event_clear = "clear";
    static constexpr const char* event_defaults = "defaults";

    static constexpr const char* css_float = "hmtl_biblio_float";
    static constexpr const char* css_red_text = "red";
    static constexpr const char* css_green_text = "green";
    static constexpr const char* css_statusbar = "html_biblio_bar";
};

#endif
