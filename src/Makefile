#*********************************************************************/
#/ webdar - a web server and interface program to libdar
#/ Copyright (C) 2013-2022 Denis Corbin
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

CC=g++
CFLAGS=-ggdb -static -Wall
CXXFLAGS=-ggdb -static -Wall `pkg-config --cflags openssl` `pkg-config --cflags libdar64` `pkg-config --cflags libthreadar` -static -D_FILE_OFFSET_BITS=64 -std=c++11


LDFLAGS=-ggdb -lstdc++ `pkg-config --libs openssl` `pkg-config --libs libdar64` `pkg-config --libs libthreadar`
LIBDAR_OBJ=

OBJ=central_report.o proto_connexion.o connexion.o ssl_connexion.o ssl_context.o exceptions.o listener.o parser.o webdar_tools.o server.o uri.o session.o date.o request.o answer.o tokens.o base64.o challenge.o choose.o css.o css_library.o html_text.o html_page.o html_table.o html_image.o html_url.o error_page.o html_form.o html_form_fieldset.o html_form_input.o html_form_radio.o html_form_select.o body_builder.o chemin.o css_property.o html_level.o html_div.o html_menu.o  html_aiguille.o saisie.o user_interface.o events.o actor.o reference.o html_url_class.o html_yes_no_box.o html_options_extract.o html_options_read.o html_crypto_algo.o html_comparison_fields.o html_options_compare.o html_options_test.o html_archive_read.o html_compression.o html_size_unit.o html_hash_algo.o html_datetime.o html_options_create.o html_archive_create.o web_user_interaction.o html_web_user_interaction.o html_button.o html_statistics.o archive_test.o webdar_css_style.o html_error.o html_libdar_running.o archive_restore.o archive_compare.o archive_create.o html_options_isolate.o archive_isolate.o html_archive_isolate.o html_options_merge.o html_archive_merge.o archive_merge.o archive_init_list.o html_dir_tree.o html_listing_page.o html_focus.o static_object.o static_object_library.o pipe_connexion.o css_class.o css_selector.o $(LIBDAR_OBJ)

all: webdar test_webdar

clean:
	rm -f *.o webdar static_object_builder static_object.sto css_selector_subtypes.hpp test_webdar

webdar: $(OBJ) webdar.o
	$(CXX) webdar.o $(OBJ) $(LDFLAGS) -o $@

test_webdar: $(OBJ) test_webdar.o
	$(CXX) $(OBJ) test_webdar.o $(LDFLAGS) -o $@


actor.o : actor.cpp actor.hpp
answer.o : answer.cpp answer.hpp
archive_compare.o : archive_compare.cpp archive_compare.hpp
archive_create.o : archive_create.cpp archive_create.hpp
archive_init_list.o : archive_init_list.cpp archive_init_list.hpp
archive_isolate.o : archive_isolate.cpp archive_isolate.hpp
archive_merge.o : archive_merge.cpp archive_merge.hpp
archive_restore.o : archive_restore.cpp archive_restore.hpp
archive_test.o : archive_test.cpp archive_test.hpp
authentication.o : authentication.cpp authentication.hpp
base64.o : base64.cpp base64.hpp
body_builder.o : body_builder.cpp body_builder.hpp
central_report.o : central_report.cpp central_report.hpp
challenge.o : challenge.cpp challenge.hpp
chemin.o : chemin.cpp chemin.hpp
choose.o : choose.cpp choose.hpp
connexion.o : connexion.cpp connexion.hpp
proto_connexion.o : proto_connexion.cpp proto_connexion.hpp
ssl_connexion.o : ssl_connexion.cpp ssl_connexion.hpp
ssl_context.o : ssl_context.cpp ssl_context.hpp
cookies.o : cookies.cpp cookies.hpp
css.o : css.cpp css.hpp
css_library.o : css_library.cpp css_library.hpp
css_property.o : css_property.cpp css_property.hpp
date.o : date.cpp date.hpp
error_page.o : error_page.cpp error_page.hpp
events.o : events.cpp events.hpp
exceptions.o : exceptions.cpp exceptions.hpp
html_aiguille.o : html_aiguille.cpp html_aiguille.hpp
html_archive_create.o : html_archive_create.cpp html_archive_create.hpp
html_archive_isolate.o : html_archive_isolate.cpp html_archive_isolate.hpp
html_archive_merge.o : html_archive_merge.cpp html_archive_merge.hpp
html_archive_read.o : html_archive_read.cpp html_archive_read.hpp
html_button.o : html_button.cpp html_button.hpp
html_comparison_fields.o : html_comparison_fields.cpp html_comparison_fields.hpp
html_compression.o : html_compression.cpp html_compression.hpp
html_crypto_algo.o : html_crypto_algo.cpp html_crypto_algo.hpp
html_datetime.o : html_datetime.cpp html_datetime.hpp
html_dir_tree.o : html_dir_tree.cpp html_dir_tree.hpp
html_div.o : html_div.cpp html_div.hpp
html_error.o : html_error.cpp html_error.hpp
html_focus.o : html_focus.cpp html_focus.hpp
html_form_fieldset.o : html_form_fieldset.cpp html_form_fieldset.hpp
html_form.o : html_form.cpp html_form.hpp
html_form_input.o : html_form_input.cpp html_form_input.hpp
html_form_radio.o : html_form_radio.cpp html_form_radio.hpp
html_form_select.o : html_form_select.cpp html_form_select.hpp
html_hash_algo.o : html_hash_algo.cpp html_hash_algo.hpp
html_image.o : html_image.cpp html_image.hpp
html_level.o : html_level.cpp html_level.hpp
html_libdar_running.o : html_libdar_running.cpp html_libdar_running.hpp
html_listing_page.o : html_listing_page.cpp html_listing_page.hpp
html_menu.o : html_menu.cpp html_menu.hpp
html_options_compare.o : html_options_compare.cpp html_options_compare.hpp
html_options_create.o : html_options_create.cpp html_options_create.hpp
html_options_extract.o : html_options_extract.cpp html_options_extract.hpp
html_options_isolate.o : html_options_isolate.cpp html_options_isolate.hpp
html_options_merge.o : html_options_merge.cpp html_options_merge.hpp
html_options_read.o : html_options_read.cpp html_options_read.hpp
html_options_test.o : html_options_test.cpp html_options_test.hpp
html_page.o : html_page.cpp html_page.hpp
html_size_unit.o : html_size_unit.cpp html_size_unit.hpp
html_statistics.o : html_statistics.cpp html_statistics.hpp
html_table.o : html_table.cpp html_table.hpp
html_text.o : html_text.cpp html_text.hpp
html_url_class.o : html_url_class.cpp html_url_class.hpp
html_url.o : html_url.cpp html_url.hpp
html_web_user_interaction.o : html_web_user_interaction.cpp html_web_user_interaction.hpp
html_yes_no_box.o : html_yes_no_box.cpp html_yes_no_box.hpp
listener.o : listener.cpp listener.hpp
parser.o : parser.cpp parser.hpp
reference.o : reference.cpp reference.hpp
request.o : request.cpp request.hpp
responder.o : responder.cpp responder.hpp
saisie.o : saisie.cpp saisie.hpp
server.o : server.cpp server.hpp
session.o : session.cpp session.hpp
static_body_builder.o : static_body_builder.cpp static_body_builder.hpp
static_object.o : static_object.cpp static_object.hpp
static_object_library.o : static_object_library.cpp static_object_library.hpp
tokens.o : tokens.cpp tokens.hpp
uri.o : uri.cpp uri.hpp
user_interface.o : user_interface.cpp user_interface.hpp
webdar_css_style.o : webdar_css_style.cpp webdar_css_style.hpp
webdar_tools.o : webdar_tools.cpp webdar_tools.hpp
web_user_interaction.o : web_user_interaction.cpp web_user_interaction.hpp
webdar.o: webdar.cpp
test_webdar.o : test_webdar.cpp
pipe_connexion.o : pipe_connexion.cpp pipe_connexion.hpp
css_class.o : css_class.cpp css_class.hpp
css_selector.o : css_selector.cpp css_selector.hpp css_selector_subtypes.hpp

static_object_library.o : static_object_library.cpp static_object.sto

# C++ generated code:
static_object.sto: static_object_batch static_object_builder
	./static_object_batch > static_object.sto

static_object_builder: static_object_builder.c

css_selector_subtypes.hpp: css_selector_generator.proto.txt css_selector_generator.routine.bash
	bash css_selector_generator.routine.bash css_selector_generator.proto.txt > $@



