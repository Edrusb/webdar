    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "webdar_tools.hpp"


    //
#include "options_create.hpp"

using namespace std;

options_create::options_create():
    form_archtype("Update"),
    fs_archtype("Type of Backup and relative options"),
    form_archgen("Update"),
    fs_archgen("General archive generation options"),
    form_shown("Update"),
    fs_shown("What to show during the operation"),
    form_perimeter("Update"),
    fs_perimeter("What to take into consideration for backup"),
    form_reading("Update"),
    fs_reading("Source file reading mode"),
    form_compr("Update"),
    fs_compr("Compression options"),
    form_slicing("Update"),
    fs_slicing("Slicing options"),
    form_crypto("Update"),
    fs_crypto("Encryption options"),
    reference("Archive of reference"),
    allow_over("Allow slice overwriting", html_form_input::check, "", 1),
    warn_over("Warn before overwriting", html_form_input::check, "", 1),
    info_details("Detailed informations", html_form_input::check, "", 1),
    pause("Pause between slices", html_form_input::number, "", 5),
    empty_dir("Store ignored directories as empty directories", html_form_input::check, "", 1),
    compression("Compression algorithm"),
    compression_level("Compression level", html_form_input::range, "", 3),
    slicing("Sliced archive", html_form_input::check, "", 1),
    slice_size("Slice size", html_form_input::number, "", 1),
    different_first_slice("Specific size for first slice", html_form_input::check, "", 1),
    first_slice_size("Slice size", html_form_input::number, "", 1),
    execute("Command to execute after each slice", html_form_input::text, "", 30),
    crypto_algo("Cipher used"),
    crypto_pass1("Pass phrase", html_form_input::password, "", 30),
    crypto_pass2("Confirm pass phrase", html_form_input::password, "", 30),
    crypto_size("Block size (?)", html_form_input::number, "", 30),
    min_compr_size("Minimum file sized compressed", html_form_input::number, "", 30),
    nodump("Avoid saving files having the 'Nodump' flag set", html_form_input::check, "", 1),
    hourshift("Hour shift", html_form_input::number, "", 5),
    empty("Dry run execution", html_form_input::check, "", 1),
    alter_atime("Alter atime reading files", html_form_input::check, "", 1),
    furtive_read_mode("Furtive read mode (if available)", html_form_input::check, "", 1),
    same_fs("Only consider files located on the same filesystem as the rooted directory", html_form_input::check, "", 1),
    snapshot("Perform a snapshot backup", html_form_input::check, "", 1),
    cache_directory_tagging("Ignore directory using a cache directory tag", html_form_input::check, "", 1),
    display_skipped("Display skipped files", html_form_input::check, "", 1),
    fixed_date("Only record files changed since:"),
    slice_permission("Slice permission", html_form_input::text, "", 1),
    slice_user_ownership("Slice user ownership", html_form_input::text, "", 1),
    slice_group_ownership("slice group ownership", html_form_input::text, "", 1),
    retry_on_change_times("Max retries saving files that changed", html_form_input::number, "", 3),
    retry_on_change_overhead("Max wasted bytes retrying saving changed files", html_form_input::number, "", 10),
    sequential_marks("Add sequential marks", html_form_input::check, "", 1),
    sparse_file_min_size("Minimum size of holes to lookup in sparse files", html_form_input::number, "", 10),
    security_check("Security warning", html_form_input::check, "", 1),
    user_comment("User comment in slice header", html_form_input::text, "", 40),
    hash_algo("Hashing algorithm"),
    slice_min_digits("Minimum digits in slice filenames", html_form_input::number, "", 5),
    ignore_unknown_inode_type("Ignore unknown inode type instead of warning", html_form_input::check, "", 1)
{
    libdar::archive_options_create defaults;

	// set field parameters

    archtype.add_choice("full", "Full backup");
    archtype.add_choice("diff", "Differential/Incremental backup");
    archtype.add_choice("snap", "Snapshot backup");
    archtype.add_choice("date", "Date based differential backup");
    compression_level.set_range(1, 9);
    if(defaults.get_reference() != NULL)
	throw WEBDAR_BUG; // not able to fill html form with an existing archive
    else
	archtype.set_selected(0);
    compression.set_no_CR();
    slice_size.set_no_CR();
    first_slice_size.set_no_CR();
    min_compr_size.set_no_CR();
    retry_on_change_overhead.set_no_CR();
    sparse_file_min_size.set_no_CR();

	// set default values
    allow_over.set_value_as_bool(defaults.get_allow_over());
    warn_over.set_value_as_bool(defaults.get_warn_over());
    info_details.set_value_as_bool(defaults.get_info_details());
    pause.set_value(libdar::deci(defaults.get_pause()).human());
    empty_dir.set_value_as_bool(defaults.get_empty_dir());
    compression.set_value(defaults.get_compression());
    compression_level.set_value(webdar_tools_convert_to_string(defaults.get_compression_level()));
    slicing.set_value_as_bool(defaults.get_slice_size() != 0);
    slice_size.set_value(libdar::deci(defaults.get_slice_size()).human());
    different_first_slice.set_value_as_bool(defaults.get_first_slice_size() != defaults.get_slice_size());
    first_slice_size.set_value(libdar::deci(defaults.get_first_slice_size()).human());
    execute.set_value(defaults.get_execute());
    crypto_algo.set_value(defaults.get_crypto_algo());
    crypto_pass1.set_value("");
    crypto_pass2.set_value("");
    crypto_size.set_value(webdar_tools_convert_to_string(defaults.get_crypto_size()));
    min_compr_size.set_value(libdar::deci(defaults.get_min_compr_size()).human());
    nodump.set_value_as_bool(defaults.get_nodump());
    what_to_check.set_value(defaults.get_comparison_fields());
    hourshift.set_value(webdar_tools_convert_to_string(defaults.get_hourshift()));
    empty.set_value_as_bool(defaults.get_empty());
    alter_atime.set_value_as_bool(defaults.get_alter_atime());
    furtive_read_mode.set_value_as_bool(defaults.get_furtive_read_mode());
    same_fs.set_value_as_bool(defaults.get_same_fs());
    snapshot.set_value_as_bool(defaults.get_snapshot());
    cache_directory_tagging.set_value_as_bool(defaults.get_cache_directory_tagging());
    display_skipped.set_value_as_bool(defaults.get_display_skipped());
    fixed_date.set_value(defaults.get_fixed_date());
    slice_permission.set_value(defaults.get_slice_permission());
    slice_user_ownership.set_value(defaults.get_slice_user_ownership());
    slice_group_ownership.set_value(defaults.get_slice_group_ownership());
    retry_on_change_times.set_value(libdar::deci(defaults.get_repeat_count()).human());
    retry_on_change_overhead.set_value(libdar::deci(defaults.get_repeat_byte()).human());
    sequential_marks.set_value_as_bool(defaults.get_sequential_marks());
    sparse_file_min_size.set_value(libdar::deci(defaults.get_sparse_file_min_size()).human());
    security_check.set_value_as_bool(defaults.get_security_check());
    user_comment.set_value(defaults.get_user_comment());
    hash_algo.set_value(defaults.get_hash_algo());
    slice_min_digits.set_value(libdar::deci(defaults.get_slice_min_digits()).human());
    ignore_unknown_inode_type.set_value_as_bool(defaults.get_ignore_unknown_inode_type());

	// build tree dependancy

	// archive type and associated optional fields
    fs_archtype.adopt(&archtype);
    fs_archtype.adopt(&reference);
    fs_archtype.adopt(&hourshift);
    fs_archtype.adopt(&snapshot);
    fs_archtype.adopt(&fixed_date);
    form_archtype.adopt(&fs_archtype);
    adopt(&form_archtype);

	// archive generation
    fs_archgen.adopt(&allow_over);
    fs_archgen.adopt(&warn_over);
    fs_archgen.adopt(&pause);
    fs_archgen.adopt(&slice_permission);
    fs_archgen.adopt(&slice_user_ownership);
    fs_archgen.adopt(&slice_group_ownership);
    fs_archgen.adopt(&retry_on_change_times);
    fs_archgen.adopt(&retry_on_change_overhead);
    fs_archgen.adopt(&retry_on_change_overhead_unit);
    fs_archgen.adopt(&sequential_marks);
    fs_archgen.adopt(&sparse_file_min_size);
    fs_archgen.adopt(&sparse_file_min_size_unit);
    fs_archgen.adopt(&user_comment);
    fs_archgen.adopt(&slice_min_digits);
    fs_archgen.adopt(&hash_algo);
    fs_archgen.adopt(&execute);
    form_archgen.adopt(&fs_archgen);
    adopt(&form_archgen);


	// operation visibility
    fs_shown.adopt(&info_details);
    fs_shown.adopt(&display_skipped);
    fs_shown.adopt(&security_check);
    fs_shown.adopt(&ignore_unknown_inode_type);
    form_shown.adopt(&fs_shown);
    adopt(&form_shown);

	// perimeter
    fs_perimeter.adopt(&empty_dir);
    fs_perimeter.adopt(&nodump);
    fs_perimeter.adopt(&what_to_check);
    fs_perimeter.adopt(&same_fs);
    fs_perimeter.adopt(&cache_directory_tagging);
    form_perimeter.adopt(&fs_perimeter);
    adopt(&form_perimeter);

	// source data
    fs_reading.adopt(&alter_atime);
    fs_reading.adopt(&furtive_read_mode);
    form_reading.adopt(&fs_reading);
    adopt(&form_reading);

	// compression
    fs_compr.adopt(&compression);
    fs_compr.adopt(&compression_level);
    fs_compr.adopt(&min_compr_size);
    fs_compr.adopt(&min_compr_size_unit);
    form_compr.adopt(&fs_compr);
    adopt(&form_compr);

	// slicing
    fs_slicing.adopt(&slicing);
    fs_slicing.adopt(&slice_size);
    fs_slicing.adopt(&slice_size_unit);
    fs_slicing.adopt(&different_first_slice);
    fs_slicing.adopt(&first_slice_size);
    fs_slicing.adopt(&first_slice_size_unit);
    form_slicing.adopt(&fs_slicing);
    adopt(&form_slicing);

	// ciphering
    fs_crypto.adopt(&crypto_algo);
    fs_crypto.adopt(&crypto_pass1);
    fs_crypto.adopt(&crypto_pass2);
    fs_crypto.adopt(&crypto_size);
    form_crypto.adopt(&fs_crypto);
    adopt(&form_crypto);

	// events and visibility
    archtype.record_actor_on_event(this, html_form_radio::changed);
    compression.record_actor_on_event(this, html_compression::changed);
    slicing.record_actor_on_event(this, html_form_input::changed);
    different_first_slice.record_actor_on_event(this, html_form_input::changed);
    crypto_algo.record_actor_on_event(this, html_crypto_algo::changed);

    on_event("");
}

void options_create::on_event(const std::string & event_name)
{
    switch(archtype.get_selected_num())
    {
    case 0: // full
	reference.set_visible(false);
	hourshift.set_visible(false);
	fixed_date.set_visible(false);
	break;
    case 1: // diff
	reference.set_visible(true);
	hourshift.set_visible(true);
	fixed_date.set_visible(false);
	break;
    case 2: // snapshot
	reference.set_visible(false);
	hourshift.set_visible(false);
	fixed_date.set_visible(false);
	break;
    case 3: // date
	reference.set_visible(false);
	hourshift.set_visible(true);
	fixed_date.set_visible(true);
	break;
    default:
	throw WEBDAR_BUG;
    }

    switch(compression.get_value())
    {
    case libdar::none:
	compression_level.set_visible(false);
	min_compr_size.set_visible(false);
	min_compr_size_unit.set_visible(false);
	break;
    case libdar::gzip:
    case libdar::bzip2:
    case libdar::lzo:
	compression_level.set_visible(true);
	min_compr_size.set_visible(true);
	min_compr_size_unit.set_visible(true);
	break;
    default:
	throw WEBDAR_BUG;
    }

    if(slicing.get_value_as_bool())
    {
	slice_size.set_visible(true);
	slice_size_unit.set_visible(true);
	different_first_slice.set_visible(true);
	if(different_first_slice.get_value_as_bool())
	{
	    first_slice_size.set_visible(true);
	    first_slice_size_unit.set_visible(true);
	}
	else
	{
	    first_slice_size.set_visible(false);
	    first_slice_size_unit.set_visible(false);
	}
    }
    else
    {
	slice_size.set_visible(false);
	slice_size_unit.set_visible(false);
	different_first_slice.set_visible(false);
	first_slice_size.set_visible(false);
	first_slice_size_unit.set_visible(false);
    }

    switch(crypto_algo.get_value())
    {
    case libdar::crypto_none:
	crypto_pass1.set_visible(true);
	crypto_pass2.set_visible(true);
	crypto_size.set_visible(true);
	break;
    case libdar::crypto_scrambling:
    case libdar::crypto_blowfish:
    case libdar::crypto_aes256:
    case libdar::crypto_twofish256:
    case libdar::crypto_serpent256:
    case libdar::crypto_camellia256:
	crypto_pass1.set_visible(false);
	crypto_pass2.set_visible(false);
	crypto_size.set_visible(false);
	break;
    default:
	throw WEBDAR_BUG;
    }
}


string options_create::get_body_part(const chemin & path,
				     const request & req)
{
    if(get_visible())
	return get_body_part_from_all_children(path, req);
    else
	return "";
}
