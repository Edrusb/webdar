CFLAGS=-ggdb -pthread `pkg-config --cflags libdar64`
CXXFLAGS=-ggdb -pthread `pkg-config --cflags libdar64`
LDFLAGS=-ggdb -lstdc++ -pthread `pkg-config --libs libdar64`

all: webdar

clean:
	rm -f *.o webdar

webdar: webdar.o central_report.o connexion.o exceptions.o listener.o thread.o parser.o webdar_tools.o server.o mutex.o semaphore.o uri.o session.o date.o request.o answer.o tokens.o base64.o challenge.o choose.o css.o body_builder.o html_text.o html_page.o html_table.o html_image.o html_url.o error_page.o html_form.o html_form_fieldset.o html_form_input.o html_form_radio.o html_form_select.o body_builder.o chemin.o css_property.o html_level.o html_div.o html_options_listing.o html_menu.o  html_aiguille.o saisie.o user_interface.o events.o reference.o html_url_class.o html_yes_no_box.o html_options_extract.o html_options_read.o html_crypto_algo.o html_comparison_fields.o html_options_compare.o options_test.o html_archive_read.o html_compression.o html_size_unit.o html_hash_algo.o html_datetime.o html_options_create.o html_archive_create.o web_user_interaction.o html_button.o html_statistics.o

webdar.o : webdar.cpp exceptions.hpp central_report.hpp listener.hpp webdar_tools.hpp server.hpp authentication.hpp
central_report.o : central_report.cpp central_report.hpp webdar_tools.hpp exceptions.hpp mutex.hpp
connexion.o : connexion.cpp connexion.hpp exceptions.hpp
exceptions.o : exceptions.cpp exceptions.hpp
listener.o : listener.cpp listener.hpp connexion.hpp server.hpp webdar_tools.hpp central_report.hpp thread.hpp exceptions.hpp authentication.hpp
thread.o : thread.cpp thread.hpp exceptions.hpp mutex.hpp
parser.o : parser.cpp parser.hpp webdar_tools.hpp exceptions.hpp connexion.hpp responder.hpp central_report.hpp date.hpp
webdar_tools.o : webdar_tools.cpp webdar_tools.hpp exceptions.hpp uri.hpp
server.o : server.cpp server.hpp exceptions.hpp central_report.hpp responder.hpp server.hpp webdar_tools.hpp cookies.hpp error_page.hpp
mutex.o : mutex.cpp mutex.hpp exceptions.hpp
semaphore.o : semaphore.cpp semaphore.hpp mutex.hpp exceptions.hpp
uri.o : uri.cpp uri.hpp
session.o : session.cpp session.hpp cookies.hpp webdar_tools.hpp responder.hpp semaphore.hpp
date.o : date.cpp date.hpp exceptions.hpp webdar_tools.hpp
tokens.o : tokens.cpp tokens.hpp
request.o : request.cpp request.hpp exceptions.hpp webdar_tools.hpp tokens.hpp
answer.o : answer.cpp answer.hpp exceptions.hpp webdar_tools.hpp uri.hpp tokens.hpp
base64.o : base64.cpp base64.hpp exceptions.hpp
challenge.o : challenge.cpp challenge.hpp base64.hpp responder.hpp authentication.hpp cookies.hpp
choose.o : choose.cpp choose.hpp session.hpp responder.hpp
css.o : css.cpp css.hpp css_property.hpp
css_property.o : css_property.cpp css_property.hpp
body_builder: body_builder.cpp body_builder.hpp
html_text.o : html_text.cpp html_text.hpp body_builder.hpp
html_page.o : html_page.cpp html_page.hpp body_builder.hpp
html_table.o : html_table.cpp html_table.hpp body_builder.hpp
html_image.o : html_image.cpp html_image.hpp body_builder.hpp
html_url.o : html_url.cpp html_url.hpp body_builder.hpp
html_form.o : html_form.cpp html_form.hpp body_builder.hpp
html_form_input.o : html_form_input.cpp html_form_input.hpp body_builder.hpp
html_form_radio.o : html_form_radio.cpp html_form_radio.hpp body_builder.hpp
html_form_fieldset.o : html_form_fieldset.cpp html_form_fieldset.hpp body_builder.hpp
error_page.o : error_page.cpp error_page.hpp
chemin.o : chemin.cpp chemin.hpp exceptions.hpp
html_level.o : html_level.cpp html_level.hpp body_builder.hpp
html_div.o : html_div.cpp html_div.hpp html_level.hpp
html_options_listing.o : html_options_listing.cpp html_options_listing.hpp body_builder.hpp html_form.hpp html_form_input.hpp exceptions.hpp
html_menu.o : html_menu.cpp html_menu.hpp body_builder.hpp html_div.hpp html_url.hpp html_text.hpp tokens.hpp exceptions.hpp
html_url_class.o : html_url_class.cpp html_url_class.hpp body_builder.hpp
html_aiguille.o : html_aiguille.cpp html_aiguille.hpp body_builder.hpp
saisie.o : saisie.cpp saisie.hpp html_url.hpp html_div.hpp html_options_listing.hpp html_aiguille.hpp html_menu.hpp
user_interface.o : user_interface.cpp user_interface.hpp saisie.hpp responder.hpp
events.o : events.cpp events.hpp exceptions.hpp
reference.o : reference.cpp reference.hpp exceptions.hpp
html_yes_no_box.o : html_yes_no_box.cpp html_yes_no_box.hpp exceptions.hpp html_form.hpp html_form_fieldset.hpp html_form_radio.hpp body_builder.hpp
html_options_extract.o : html_options_extract.cpp html_options_extract.hpp html_form.hpp html_form_fieldset.hpp html_form_input.hpp html_form_select.hpp
html_options_read.o : html_options_read.cpp html_options_read.hpp body_builder.hpp html_div.hpp html_form.hpp html_form_fieldset.hpp html_form_select.hpp html_form_input.hpp
html_crypto_algo.o : html_crypto_algo.cpp html_crypto_algo.hpp html_form_select.hpp body_builder.hpp
html_comparison_fields.o : html_comparison_fields.cpp html_comparison_fields.hpp html_form_select.hpp
html_options_compare.o : html_options_compare.cpp html_options_compare.hpp body_builder.hpp html_form_input.hpp html_form_fieldset.hpp html_form.hpp html_comparison_fields.hpp
options_test.o : options_test.cpp options_test.hpp body_builder.hpp html_form.hpp html_form_fieldset.hpp html_form_input.hpp
html_archive_read.o : html_archive_read.cpp html_archive_read.cpp body_builder.hpp html_div.hpp html_form.hpp html_form_fieldset.hpp html_form_select.hpp html_form_input.hpp html_crypto_algo.hpp events.hpp
html_compression.o : html_compression.cpp html_compression.hpp html_form_select.hpp
html_size_unit.o : html_size_unit.cpp html_size_unit.hpp html_form_select.hpp
html_hash_algo.o : html_hash_algo.cpp html_hash_algo.hpp html_form_select.hpp
html_datetime.o : html_datetime.cpp html_datetime.hpp html_form_input.hpp html_form_select.hpp body_builder.hpp
options_create.o : html_options_create.cpp html_options_create.hpp body_builder.hpp html_compression.hpp html_crypto_algo.hpp html_form_input.hpp html_comparison_fields.hpp html_size_unit.hpp html_form_fieldset.hpp html_datetime.hpp html_form.hpp html_archive_read.hpp html_hash_algo.hpp
html_archive_create.o : html_archive_create.cpp html_archive_create.hpp body_builder.hpp html_form.hpp html_form_input.hpp html_options_create.hpp
web_user_interaction.o: web_user_interaction.cpp web_user_interaction.hpp body_builder.hpp html_form_input.hpp html_form.hpp html_form_radio.hpp html_form_fieldset.hpp html_text.hpp events.hpp semaphore.hpp
html_button.o : html_button.cpp html_button.hpp body_builder.hpp events.hpp html_url_class.hpp html_url.hpp html_div.hpp
html_statistics.o : html_statistics.cpp html_statistics.hpp body_builder.hpp html_table.hpp html_text.hpp
