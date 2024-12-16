########################################################################
# webdar - a web server and interface program to libdar
# Copyright (C) 2013-2024 Denis Corbin
#
# This file is part of Webdar
#
#  Webdar is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Webdar is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
#
#----
#  to contact the author: dar.linux@free.fr
########################################################################


#
# conditionnal part UPX
#

if USE_UPX

install-exec-hook :
	cd $(DESTDIR)$(bindir) ; $(UPX_PROG) -9 webdar$(EXEEXT) $(ALSO_BUILD_INST) || exit 0

else
install-exec-hook:
	echo "no executable packing, UPX not found or disabled"
endif



#
# unconditionnal part
#

bin_PROGRAMS = webdar
noinst_PROGRAMS = static_object_builder

static_object_builder_SOURCES = static_object_builder.c

static_object.sto: static_object_batch static_object_builder
	./static_object_batch > static_object.sto

dist_noinst_DATA = static_object_batch

all-local: static_object.sto

clean-local:
	rm -f static_object.sto

COMMON = central_report.cpp central_report.hpp proto_connexion.cpp proto_connexion.hpp connexion.cpp connexion.hpp authentication.hpp cookies.hpp ssl_connexion.cpp ssl_connexion.hpp ssl_context.cpp ssl_context.hpp exceptions.cpp exceptions.hpp listener.cpp listener.hpp parser.cpp parser.hpp webdar_tools.cpp server.cpp server.hpp webdar_tools.hpp webdar_tools.hpp uri.cpp uri.hpp session.cpp session.hpp date.cpp date.hpp request.cpp request.hpp answer.cpp answer.hpp tokens.cpp tokens.hpp base64.cpp base64.hpp challenge.cpp challenge.hpp choose.cpp choose.hpp css.cpp css.hpp css_library.cpp css_library.hpp html_text.cpp html_text.hpp html_page.cpp html_page.hpp html_table.cpp html_table.hpp html_image.cpp html_image.hpp html_url.cpp html_url.hpp responder.hpp error_page.cpp error_page.hpp html_form.cpp html_form.hpp html_form_fieldset.cpp html_form_fieldset.hpp html_form_input.cpp html_form_input.hpp html_form_radio.cpp html_form_radio.hpp html_form_select.cpp html_form_select.hpp body_builder.cpp body_builder.hpp static_body_builder.hpp chemin.cpp chemin.hpp css_property.cpp css_property.hpp html_level.cpp html_level.hpp html_div.cpp html_div.hpp html_menu.cpp html_menu.hpp html_aiguille.cpp html_aiguille.hpp saisie.cpp saisie.hpp user_interface.cpp user_interface.hpp events.cpp events.hpp actor.cpp actor.hpp reference.cpp reference.hpp html_yes_no_box.cpp html_yes_no_box.hpp html_options_extract.cpp html_options_extract.hpp html_options_read.cpp html_options_read.hpp html_crypto_algo.cpp html_crypto_algo.hpp html_comparison_fields.cpp html_comparison_fields.hpp html_options_compare.cpp html_options_compare.hpp html_options_test.cpp html_options_test.hpp html_archive_read.cpp html_archive_read.hpp html_compression.cpp html_compression.hpp html_size_unit.cpp html_size_unit.hpp html_hash_algo.cpp html_hash_algo.hpp html_datetime.cpp html_datetime.hpp html_options_create.cpp html_options_create.hpp html_archive_create.cpp html_archive_create.hpp web_user_interaction.cpp web_user_interaction.hpp html_web_user_interaction.cpp html_web_user_interaction.hpp html_button.cpp html_button.hpp html_statistics.cpp html_statistics.hpp archive_test.cpp archive_test.hpp html_error.cpp html_error.hpp html_libdar_running_page.cpp html_libdar_running_page.hpp archive_restore.cpp archive_restore.hpp archive_compare.cpp archive_compare.hpp archive_create.cpp archive_create.hpp html_options_isolate.cpp html_options_isolate.hpp archive_isolate.cpp archive_isolate.hpp html_archive_isolate.cpp html_archive_isolate.hpp html_options_merge.cpp html_options_merge.hpp html_archive_merge.cpp html_archive_merge.hpp archive_merge.cpp archive_merge.hpp archive_init_list.cpp archive_init_list.hpp html_dir_tree.cpp html_dir_tree.hpp html_listing_page.cpp html_listing_page.hpp html_focus.cpp html_focus.hpp static_object.cpp static_object.hpp static_object_library.cpp static_object_library.hpp pipe_connexion.cpp pipe_connexion.hpp css_class.cpp css_class.hpp webdar_css_style.cpp webdar_css_style.hpp css_class_group.cpp css_class_group.hpp html_tabs.cpp html_tabs.hpp html_select_file.cpp html_select_file.hpp html_popup.cpp html_popup.hpp html_form_input_file.cpp html_form_input_file.hpp jsoner.hpp jsoner.cpp html_derouleur.cpp html_derouleur.hpp html_entrepot.hpp html_entrepot.cpp environment.hpp environment.cpp html_libdar_running_popup.hpp html_libdar_running_popup.cpp html_mask.hpp html_mask.cpp html_form_mask_expression.hpp html_form_mask_expression.cpp html_form_mask_bool.hpp html_form_mask_bool.cpp html_mask_form_filename.hpp html_mask_form_filename.cpp html_form_mask_subdir.hpp html_form_mask_subdir.cpp html_mask_form_path.hpp html_mask_form_path.cpp html_double_button.hpp html_double_button.cpp html_demo.hpp html_demo.cpp html_form_mask_file.hpp html_form_mask_file.cpp html_archive_repair.cpp html_archive_repair.hpp archive_repair.hpp archive_repair.cpp html_archive_compare.hpp html_archive_compare.cpp html_archive_extract.hpp html_archive_extract.cpp html_options_repair.hpp html_options_repair.cpp html_overwrite_action.hpp html_overwrite_static_action.hpp html_form_overwrite_constant_action.hpp html_form_overwrite_constant_action.cpp html_overwrite_criterium.hpp html_form_overwrite_base_criterium.hpp html_form_overwrite_base_criterium.cpp html_form_overwrite_combining_criterium.hpp html_form_overwrite_combining_criterium.cpp html_form_overwrite_condition_action.hpp html_form_overwrite_conditional_action.cpp html_form_overwrite_action.hpp html_form_overwrite_action.cpp html_hr.hpp html_hr.cpp html_form_overwrite_chain_action.hpp html_form_overwrite_chain_action.cpp html_form_dynamic_table.hpp html_form_dynamic_table.cpp html_form_gnupg_list.hpp html_form_gnupg_list.cpp html_form_overwrite_chain_cell.hpp html_form_overwrite_chain_cell.cpp html_legend.hpp html_legend.cpp html_form_same_fs.cpp html_form_same_fs.hpp html_form_ignore_as_symlink.hpp html_form_ignore_as_symlink.cpp html_form_sig_block_size.hpp html_form_sig_block_size.cpp html_form_input_unit.hpp html_form_input_unit.cpp html_compression_params.hpp html_compression_params.cpp html_slicing.hpp html_slicing.cpp html_ciphering.hpp html_ciphering.cpp html_fsa_scope.hpp html_fsa_scope.cpp html_disconnect.hpp html_disconnect.cpp disconnected_page.hpp disconnected_page.cpp server_pool.hpp server_pool.cpp html_option_list.hpp html_options_list.cpp html_summary_page.hpp html_summary_page.cpp bibliotheque.cpp bibliotheque.hpp

webdar_SOURCES = $(COMMON) webdar.cpp
webdar_CPPFLAGS = $(LIBDAR_CFLAGS) $(OPENSSL_CFLAGS)
webdar_LDFLAGS = $(LIBDAR_LIBS) $(OPENSSL_LIBS)

static_object_library.cpp: static_object.sto
