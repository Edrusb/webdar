#*********************************************************************/
#/ webdar - a web server and interface program to libdar
#/ Copyright (C) 2013 Denis Corbin
#/
#/ This file is part of Webdar
#/
#/  Webdar is free software: you can redistribute it and/or modify
#/  it under the terms of the GNU General Public License as published by
#/  the Free Software Foundation, either version 3 of the License, or
#/  (at your option) any later version.
#/
#/  Webdar is distributed in the hope that it will be useful,
#/  but WITHOUT ANY WARRANTY; without even the implied warranty of
#/  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#/  GNU General Public License for more details.
#/
#/  You should have received a copy of the GNU General Public License
#/  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
#/
#/----
#/  to contact the author: dar.linux@free.fr
#*********************************************************************/

CFLAGS=-ggdb -static
CXXFLAGS=-ggdb `pkg-config --cflags libdar64` `pkg-config --cflags libthreadar` -static -D_FILE_OFFSET_BITS=64 -std=c++11



#LDFLAGS=-ggdb -lstdc++ -lpthread -lattr -lgcrypt -lgpg-error -llzo2 -lbz2 -lz -ldl
#LIBDAR_OBJ=../../../../DAR/dar/src/libdar/*.o

LDFLAGS=-ggdb -lstdc++ -pthread `pkg-config --libs libdar64` `pkg-config --libs libthreadar`
LIBDAR_OBJ=

all: webdar

clean:
	rm -f *.o webdar static_object_builder static_object.sto

webdar: webdar.o central_report.o connexion.o exceptions.o listener.o parser.o webdar_tools.o server.o uri.o session.o date.o request.o answer.o tokens.o base64.o challenge.o choose.o css.o body_builder.o html_text.o html_page.o html_table.o html_image.o html_url.o error_page.o html_form.o html_form_fieldset.o html_form_input.o html_form_radio.o html_form_select.o body_builder.o chemin.o css_property.o html_level.o html_div.o html_menu.o  html_aiguille.o saisie.o user_interface.o events.o reference.o html_url_class.o html_yes_no_box.o html_options_extract.o html_options_read.o html_crypto_algo.o html_comparison_fields.o html_options_compare.o html_options_test.o html_archive_read.o html_compression.o html_size_unit.o html_hash_algo.o html_datetime.o html_options_create.o html_archive_create.o web_user_interaction.o html_web_user_interaction.o html_button.o html_statistics.o archive_test.o webdar_css_style.o html_error.o html_libdar_running.o archive_restore.o archive_compare.o archive_create.o html_options_isolate.o archive_isolate.o html_archive_isolate.o html_options_merge.o html_archive_merge.o archive_merge.o archive_init_list.o html_dir_tree.o html_listing_page.o html_focus.o static_object.o static_object_library.o $(LIBDAR_OBJ)

static_object_library.o : static_object_library.cpp static_object.sto

# C++ generated code:
static_object.sto: static_object_batch static_object_builder
	./static_object_batch > static_object.sto

static_object_builder: static_object_builder.o

