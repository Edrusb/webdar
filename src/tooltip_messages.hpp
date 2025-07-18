/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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

#ifndef TOOLTIPS_MESSAGES_HPP
#define TOOLTIPS_MESSAGES_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    // common messages
#define TOOLTIP_COMMON_INFO_DETAILS "Show information about the archive layer creation, slicing generation, but nothing about the treated files and directories"
#define TOOLTIP_COMMON_DISPLAY_SKIPPED "Display files that have been excluded from the operation. If a directory is excluded from the backup, all its content is excluded and nothing is reported further about that directory and its content"
#define TOOLTIP_COMMON_DISPLAY_TREATED "Display information about files an directories while they are treated"
#define TOOLTIP_COMMON_DISPLAY_ONLY_DIR "Restrict the information about the treated directories only, a line is displayed when the backup process enters each directory"

    // webdar headers

    // arriere_boutique template class

#define TOOLTIP_AB_CONFIG_NAME "Name to use to save the configuration displayed above. You can also change it to save a copy under another name (\"save as\" feature). However you cannot save under the name of an existing configuration. To edit an existing configuration select it from the list here on the right, click the \"load selected\" button, perform changes in the configuration and get back to this save button without changing the name of the configuration"
#define TOOLTIP_AB_SELECT "Select an existing configuration, then load it. You can then either edit it on the left or delete it from the configuration using the \"delete\" button below"

    // guichet class

#define TOOLTIP_G_SELECT_SUBCOMP "Select an existing configuration. Note, this configuration will not be removable until it is not referred from here nor from any other similar places"
#define TOOLTIP_G_SELECT "Select an existing configuration defined in the \"Configure\" menu or use the \"manual config\" choice to set it up in-place, you will be able to save it also as a new configuration using the \"save/save as\" button right below. Warning you may have nested configuration parts each with its \"save/save as\" button, check the indentation and vertical line on the left to determine what configuration part you want to add to the \"configure\" menu"
#define TOOLTIP_G_SAVE "The current configuration is anonymous, from here you use it as is, modify it or save as a new configuration. To edit an existing configuration and save changes under that same name, go to the \"Configure\" menu"

    // html_bibliotheque class

#define TOOLTIP_HB_FILENAME "Filename where the configuration has been read from and where it will be saved to. The configuration saved includes all tabs of the present page and only them, nothing is saved from the \"Create\" menu for example"
#define TOOLTIP_HB_AUTOSAVE "If this box is unchecked, any configuration change is just kept in memory, until you click on the \"save\" button just below, at which time the configuration as seen from all tabs is saved to the file which name is provided above. A status message shows at the top of the page telling whether the current configuration in memory has been saved to file or not. If checked, this operation is performed automatically upon change and the status message on top page does not show."
#define TOOLTIP_HB_UPLOAD "First select the configuration file your browser will upload to Webdar, then click the \"upload\" button"

    // html_ciphering class

#define TOOLTIP_HC_TYPE "Symmetric encryption means here the use of a password to cipher and decipher the backup, while asymmetrical encryption means the use of public/private GPG key pair, one key to cipher and eventually sign, the other to decipher the backup"
#define TOOLTIP_HC_ALGO "Ciphering algoritm, most of the time you will want to use AES"
#define TOOLTIP_HC_PASS1 "The password or passphrase to use to cipher the backup (and to remember in order to decipher it)"
#define TOOLTIP_HC_PASS2 "Retype the same password or passphrase as entered above"
#define TOOLTIP_HC_SIZE "Keep this parameter to its default value (10 kio), else you will have to remember its value and set it back at the same value in order to decipher the backup"
#define TOOLTIP_HC_THREADS "Number of threads to use to cipher or decipher the data"
#define TOOLTIP_HC_FS_KDF_HASH "Humans choose password/phrase in a very restrictive name space. The Key Derivation Function, takes this pass and a salt (an arbitrarily chosen sequence stored in clear in the archive header) as input of a time consuming transformation process to obtain a more randomized encryption key. With the salt, different archives use different encryption keys even if the human provided password is the same, while the CPU intensive transformation makes dictionnary attack more costly"
#define TOOLTIP_HC_KDF_HASH0 "md5 is not a strong hash algorithm and it outputs in a small namespace. Avoid using it for KDF, it is here to support old archive formats"
#define TOOLTIP_HC_KDF_HASH1 "sha1 is also here to support old archive format, don't use it anymore"
#define TOOLTIP_HC_KDF_HASH2 "sha512 can be used with a high number of iteration for the time to get the ciphering key from salt and pass to take a few seconds on an average computer"
#define TOOLTIP_HC_KDF_HASH3 "whirlpool has a very large output namespace, though use it with enough iteration for the KDF process to take also several seeconds on an average computer"
#define TOOLTIP_HC_KDF_HASH4 "argon2 is known as the reference for KDF function, this is the default and best choice according to literature since 2015 and at least up to year 2023"
#define TOOLTIP_HC_ITERATION_COUNT "The hash result of the salt and pass is hashed several times in loop, for the process of getting a randomized string outside the human usual namespace to cost CPU resource and time and drive an dictionnary attack slow and/or costly in time and or CPU"

    // html_compression_params class

#define TOOLTIP_HCP_COMPR "The algorithms differ in execution time to compress and to decompress, as well as the average compression ratio obtained depending on the nature of the data to compress, choose the algorithm that best fits your use case,"
#define TOOLTIP_HCP_COMPR_LEVEL "The higher this number is, the more CPU and time will be spend to reach the best compression ratio and lower the storage footprint"
#define TOOLTIP_HCP_MIN_COMPR "Files under that size will never be compressed"
#define TOOLTIP_HCP_COMPR_BLOCK "If set to zero, use stream compression which gives the best compression result but cannot use several threads. Else, split the data to compress in given block size and compress those block independently, something that can be spread between different threads, though it increases the memory and CPU footprint, at most by the number of threads set: small files may not mobilize all threads if the number of blocks they contain is less than the number of available threads"
#define TOOLTIP_HCP_NEVER_RESAVE "When compressed data of a file uses more space than uncompressed, by default it is stored uncompressed, which means re-reading it again and overwriting the comprressed data written to the backup. If checked, keep data compressed in any case"
#define TOOLTIP_HCP_THREADS "The number of threads for data compression. Only one thread will be created and used when not using block compression"
#define TOOLTIP_HCP_KEEP_COMPR "For merging operation only, do not try to decompress and recompress files that are retained in the resulting archive. If checked, the current configuration will be refused for other operations, like backup or isolation"

    // html_demo class

#define TOOLTIP_D_LEFT0 "the box above is hidden from the user, but its value is kept recorded"
#define TOOLTIP_D_LEFT1 "the box above is visibile but cannot be modified by the user"
#define TOOLTIP_D_LEFT2 "the box is editable and visible (which most of the time is the default setting)"
#define TOOLTIP_D_LI "enter text here"
#define TOOLTIP_D_RI "this is the information known by webdar at this point in time"
#define TOOLTIP_D_COUNTER "This shows the provided string size as computed at the time the \"Calculate\" button has last been pressed"

    // html_entrepot class

#define TOOLTIP_HE_REPO_TYPE "type \"local\" is where webdar is running, other type should point to an existing and reachable server from the node where webdar is running"
#define TOOLTIP_HE_HOST "hostname or IP address"
#define TOOLTIP_HE_PORT "TCP port the service listens on"
#define TOOLTIP_HE_LOGIN "login name to connect to the remote server"
#define TOOLTIP_HE_PASS "password to use to authenticate the login user on that remote server"
#define TOOLTIP_HE_AUTH "To authenticate using a keyfile, provided a key pair, which public key has been added to the \"~/.ssh/authorized_keys\" for that login account on that remote server. See ssh documentation for more info"
#define TOOLTIP_HE_PUB "This file should contain the public key part of the key pair"
#define TOOLTIP_HE_PRV "This file should contain the private key counterpart of the public key file"
#define TOOLTIP_HE_KNOWN_CHECK "To avoid connecting to a pirate server, ssh record the server key fingerprint during the first connection and store them in a file named known_hosts. Upon a new connection to to a know server, if the server fingerprint has changed ssh refuse to connect and let you send your credentials and transfer data to a potential wrong server. You can disable this check assuming you know what you are doing!"
#define TOOLTIP_HE_KNOWN_FILE "It is possible change from the default location of the known_hosts file, the common location is provided here by default"
#define TOOLTIP_HE_WAIT "Webdar does not stop upon a network issue, like a lack of connectivity or disconnection from the remote server, it waits this amount of seconds and retry until it succeeds or is interrupted by the webdar user. Choosing too short delay may overload the remote server and defeat the delay feature that is here to let the server address overload and proceed later when more resources are available"
#define TOOLTIP_HE_VERBOSE "For troubleshooting purpose, verbose information can be displayed on command-line interface where webdar has been launched from"

    // html_entrepot_landing class

#define TOOLTIP_HEL_USE "If checked, when this repository is selected for an operation, the repository path is replaced by the landing path value provided here"
#define TOOLTIP_HEL_PATH "A landing path is a path on the remote server where you want webdar to change to, at connection time. It is expected to be the place where, by default, you store your backups on that server. It will be used by default when no path has yet been set for an operation"

    // html_form_mask_expression class

#define TOOLTIP_HFME_TYPE "Glob expression is expression you commonly have under a shell with special characters like * ? and []"
#define TOOLTIP_HFME_NEGATE "When a file matches the mask consider it does not and vice versa"
#define TOOLTIP_HFME_EXPRESSION "Expression, eventually with wildcards (pay attention to the mask type above) that a file must satisfy to be considered in the operation"

    // html_form_mask_file class

#define TOOLTIP_HFMF_FILENAME "filename containing a list of path+filename to include or exclude from the operation"
#define TOOLTIP_HFMF_EXCLUDE "If checked, entries listed in the provided are excluded from the operation, else there are included into the operation"
#define TOOLTIP_HFMF_CASESENSIT "Consider case sensitivity or not for the file listing content in the provided filename"

    // html_form_mask_subdir class

#define TOOLTIP_HFMS_TYPE "Pay attention, only exluding mask support mask (glob and regex), in included path no character has a special meaning"
#define TOOLTIP_HFMS_CASE "case sensitivity when comparing this mask with file to process"
#define TOOLTIP_HFMS_REGEX "If unchecked the provided mask is assumed a glob expression, else if checked it is assumed a regular expression"
#define TOOLTIP_HFMS_MASK "subdirectory to match against. Pay attention, absolute path are not allowed in all context, the use of such configuration will not be accepted in that cases"

    // html_form_overwrite_action class

#define TOOLTIP_HFOA_TYPE "Select the type of action and click the update button"

    // html_form_overwritre_base_criterium class
#define TOOLTIP_HFOBC_NEGATE "Take the opposite result of the selected criterium, for example if the selected criterium is \"in place is an inode\" negating it will result in \"in place is *not* an inode\""
#define TOOLTIP_HFOBC_INVERT "Invert the roles of the \"in-place\" entry and the \"to be added\" one, for example if the selected criterium is \"in-place is an inode\" inverting will result in \"*to be added* is an inode\""
#define TOOLTIP_HFOBC_BASE "Define what action to do when the same entry is found in the \"in-place\" archive and in the \"to be added\" one"


    // html_form_overwrite_chain_action class

#define TOOLTIP_HFOCA_FS "In a chain of action, evaluations are performed one after the other; if a decision is set for Data or for EA it is not changed by subsequent action, the use of the \"keep undefined\" decision will be of most use inside the first cells of a chain of action"

    // html_form_overwrite_constant_action class

#define TOOLTIP_HFOCA_DATA "Defines the overwriting policy for the data, file's data, pointed to file of a symlink, major and minor of a char or block device, it also concerns permissions, ownership and dates of any inode type"
#define TOOLTIP_HFOCA_EA "Defines the overwriting policy for the Extended Attributes (EA) and File Specific Attributes (FSA) of any inode type"

    // html_form_sig_block_size

#define TOOLTIP_HFSB_DELTA "Better not calculating delta signature but resaving as a whole small files upon change, this saves both CPU cycles and storage requirments"
#define TOOLTIP_HFSB_FUNCTION "Delta signature is what is used to compare with the previous status of a file. It is calculated per block of a file's data. The larger the block is, the less blocks will be, resulting in a smaller delta signature for a given file size, but the more data will have to be resaved upon binary change inside that file, as the block size is bigger"
#define TOOLTIP_HFSB_FUNCTION0 "Block size is independent from the file sizes, consider adjusting the multiplicative and/or divisor factors below. Delta signature will grow proportionnaly to the file size"
#define TOOLTIP_HFSB_FUNCTION1 "Block size increases very slowly when the file size increases. This bring some advantage compare to the linear function to reduce the delta signature size for big files without much impact on the amount of data to resave upon change"
#define TOOLTIP_HFSB_FUNCTION2 "Block size increases quicker than logarithmic based function, when the file size increases"
#define TOOLTIP_HFSB_FUNCTION3 "Block size increases even quicker when the file size increases, but less faster than the linear function"
#define TOOLTIP_HFSB_FUNCTION4 "Block size increase proportionnaly to the size of the file, consider setting the divisor factor to a value high enough, for example greater than 10, to avoid having too few or even a single block per file. Binary delta signature should almost have the same size whetever is the file size"
#define TOOLTIP_HFSB_MULTIPLY "Adjust the formula by multiplying it by a fixed factor. Click the \"Update\" button and look at the new formula expression above"
#define TOOLTIP_HFSB_DIVISOR "Adjust the formula by dividing it with a fixed factor. Can be combined with the multiplicative factor too"
#define TOOLTIP_HFSB_MIN_SIZE "Bypass the result of the formula described above for the delta signature block size not to be smaller than this provided value"
#define TOOLTIP_HFSB_MAX_SIZE "Bypass the result of the formula described above for the delta signature block size not to be larger than this provided value"

    // html_fsa_scope class

#define TOOLTIP_HFS_FS "Some attributes are specific to some filesystems, like the immutable flag for Linux ext2/3/4 or the birth or creation date on MacOS HFS+ filesystem. Webdar can save and restore them, and when supported by the filesystem. However if you restore accross different filesystem types, you may get a lot of warnings. You have the option to ignore them at restoration time or, here, to directly avoid backing them up"

    // html_options_create class

#define TOOLTIP_HOC_ARCHTYPE0 "Save all files and directories under the different file and path based filters, perimeters, etc. as defined in the following sections"
#define TOOLTIP_HOC_ARCHTYPE1 "Only saves files that have changed since the backup given as reference has been made. This backup of reference may be a full backup or another incrementa/differential backup as the one about to create here, with that option set. Of course here too all file and path based filters, perimters, etc. also apply on top"
#define TOOLTIP_HOC_ARCHTYPE2 "Same as previously, with the addition to not save the modified files in totality, but leverage delta signatures to only record the portions of the files that changed. Note: at restoration time you will need to restore the files from the previous backup then restore this backup on top to apply a binary patch on files saved that way"
#define TOOLTIP_HOC_ARCHTYPE3 "Make a reference of the current filesystem status without saving any file. A snapshot can be used as a reference for a differential backup or just be used to check whether existing files at the time of the snapshot have changed since the snapshot was made"
#define TOOLTIP_HOC_ARCHTYPE4 "Only change files modified after a given date. This creates a differential backup without requiring a backup of reference"
#define TOOLTIP_HOC_WTC "How to consider a file has changed since the backup of reference was made"
#define TOOLTIP_HOC_HOURSHIFT "On some systems (mainly under Windows) the daylight saving time is performed by modifing file dates, not just the way the clock is displayed on the system. This leads to save files that have not changed when passing to or from summer time. If set to 1 or more, a file will not be considered to have changed if only its date changed and by an exact number of hours"
#define TOOLTIP_HOC_BIN_DELTA "To add delta signature to a backup, see below the \"Delta Signatures\" section"
#define TOOLTIP_HOC_FURTIVE_READ "Reading a file modifies the last acces time, or if after reading it, the last access time (atime) is set back to the value it has before the file was read, the system changes the last change time (ctime) which cannot be modified from an application, like webdar. Linux system provides a privileged mode to access a file without modifying neither atime nor ctime. Webdar must be root or have the proper capability set to activate this mode. If activation failed, webdar will default to the mode you chose (modifying atime is the most common setting)"
#define TOOLTIP_HOC_ZEROING_DATE "Some filesystems support negative dates, which means dates before year 1970. Webdar cannot handle such negative dates, but can consider these as zero, which corresponds to the end of the last second of year 1969"
#define TOOLTIP_HOC_MOD_DATA_DETECT "In the context of differential/incremental backup, normally the data of a file is considered to have changed if the mtime (last modification time) has changed. In libdar older versions, a file was completely resaved if any date, permission or ownship of a file had changed. Since libdar 2.6.0 those metadata change (permission, ownership, atime, ctime...) can be saved without having to resave the whole file content, this reduces a lot of the backup size and shorten the backup process"
#define TOOLTIP_HOC_ALLOW_OVER "If unchecked, this prevents webdar to overwrite slices of a previous backup having the same name, else unless disabled below, a warning and confirmation request is issued before overwriting"
#define TOOLTIP_HOC_WARN_OVER "Warn and ask for confirmation before overwriting slices of an existing backup using the same name as the one we are about to create"
#define TOOLTIP_HOC_PAUSE "Pause webdar after each 'N' slice, a confirmation is then asked to continue the backup process for the next N slices before pausing again. The objective of that feature was to treat the generated slices like transfering them through the network or at earlier time to burn it or them to a CD or DVD disc"
#define TOOLTIP_HOC_RETRY_TIMES "Right before saving a file, webdar record its last modification date, if right after the backup of that file this date has changed, dar retries to save it again and so forth if the date changed again, but for a limited amount of times. Passed that, the file is flagged as \"dirty\" meaning the file content may be inconsistent regarding what a user application may expect to find in it. At restoration time you will have different options on how to treat dirty files"
#define TOOLTIP_HOC_RETRY_OVER "Resaving a changed file does not consume additional data in normal condition. This is not the case when saving to a pipe where it is not possible to overwrite the new version of the file in the generated backup. This parameter let define a maximum amount of bytes that are wasted that way during the overall backup process, after which no data retry is performed and files are directly flagged as dirty if they changed while they were read for backup"
#define TOOLTIP_HOC_SEQ_READ "You should not disable sequential marks unless you know what you are doing. These marks (sometime also called tape marks) provides a redundant method to read a backup, in particular in case of filesystem saturation during a backup process; they may also help recorver a backup that has been corrupted on disk or tape"
#define TOOLTIP_HOC_SPARSE_MIN "Long sequences of zeroed bytes in a file may be stored as a \"hole\" on the filesystem, leading to no or very little disk space consumtion, while such file can span several terabytes, for example. Dar can detect those holes and record them as a short data structures in the backup. Even without compression, this saved a lot of space in the resulting backup and let dar recreate the so called holes at restoration time avoiding creating a huge file when it can leverage the underlying filesystem ability to handle sparse files. It does not make sense to set a value below 15 bytes, but you can disable this feature setting it to zero, this will increase the backup speed at the cost of the inability to store an restore sparse files"
#define TOOLTIP_HOC_USER_COMMENT "This field is available for the user to add any information but pay attention, this information is stored in clear in the backup header even if the backup is ciphered"
#define TOOLTIP_HOC_HASH_ALGO "With this option a hash of the generated slices is computed in memory during the slice creation and written to a separated file beside the slice to let you check that no data alteration came on disk. Use the -c option of md5sum, sha1sum, sh1512sum or rhash -W command to easily check those hashes. For example: \"rhash -W -c slice.1.dar.whirlpool\""
#define TOOLTIP_HOC_EXECUTE "Execute a shell command after each slice completion"
#define TOOLTIP_HOC_EMPTY "Perform the backup operation but send the generated backup bytes into black hole, leading no backup to be created at all, though you can get messages from the filesystem reading process and webdar filter mechanisms"
#define TOOLTIP_HOC_INFO_DETAILS TOOLTIP_COMMON_INFO_DETAILS
#define TOOLTIP_HOC_DISPLAY_TREATED TOOLTIP_COMMON_DISPLAY_TREATED
#define TOOLTIP_HOC_DISPLAY_ONLY_DIR TOOLTIP_COMMON_DISPLAY_ONLY_DIR
#define TOOLTIP_HOC_DISPLAY_SKIPPED TOOLTIP_COMMON_DISPLAY_SKIPPED
#define TOOLTIP_HOC_DISPLAY_DIR_SUMMARY "Display a summary line about the amount of saved data and its compression ratio at the end of each treated directory"
#define TOOLTIP_HOC_SECU_CHECK "When a file has only changed by its ctime since the backup of reference was made, with that option webdar triggers a security warning. This may be expected when for example changing a file permission and setting it back to the previous value, it may also be a sign of concealment when one tries to hide changes made to a file. If such warnings occur too often, use an anti-virus or rootkit scanner and disable this feature in webdar"
#define TOOLTIP_HOC_IGNORE_UNKNOWN "When dar finds an inode type it does not know, a warning is issued. You can disable this warning by checking this option, no warning will show about those unknown inode type and the backup process will complete without considering them at all"
#define TOOLTIP_HOC_EMPTY_DIR "When a directory is excluded from a backup, as expected it does not show in the backup and is thus not restored. When this concerns a mount point for virtual filesystems (like /proc, /sys, /dev and mounted tmpfs often found for /tmp and /run), this option let webdar store any excluded directory as an empty directory, including those mount points you'd be advise not to backup.  Then, at restoration time, webdar will automatically recreate the mount points for those virtual filesystems you have excluded from your backup"
#define TOOLTIP_HOC_CACHE_TAG "Many applications use temporary data on disk for caching purposes and flag those directories following the cache directory tagging standard. Webdar understands that and can avoid saving directories tagged that way, when this option is checked"
#define TOOLTIP_HOC_NODUMP "Some filesystem let users and applications set a 'nodump' flag on files, meaning they should not be backed up. Dar can exclude those files from a backup, if this option is checked"
#define TOOLTIP_HOC_EXCLUDE_BY_EA "In complement to the nodump and cache directory tagging way to exclude files, webdar proposes the use of a well-defined Extended Attribute to flag files and directories to be exclude from the backup"
#define TOOLTIP_HOC_DEFAULT_EA "Use the default Extended Attribute (if this option is set, its value shows below in the disabled box)"
#define TOOLTIP_HOC_EXCLUDE_BY_EA_NAME "Specify here the EA name of your choice and flag files accordingly with that EA for webdar not to save them"
#define TOOLTIP_HOC_SAME_FS "The whole filesystem where the below specified directories reside will be either include or excluded from the backup. if only included filesystem are specified, none of the other filesystem will be looked at for backup, though the filesystem where is located the path defined as \"filesystem root\" for a backup, is never excluded. If only excluded directories are specified, any other filesystem will be considered for backup (applying other filtering mecanismes you have set). If there is a mix of included and excluded filesystems only included filesystem that have not also been specified to be excluded will be considered for the backup operation"

    // html_options_read class

#define TOOLTIP_HOR_ALGO "Unless reading a very old archive, use \"none\" here even if the backup is ciphered, algorithm is auto-detected"
#define TOOLTIP_HOR_PASS "If not specified here, password/passphrase will be asked later. Pay attention that setting password here will lead it to be saved unencrypted in webdar configuration file"
#define TOOLTIP_HOR_SIZE "keep the default 10 kio unless you have changed it at backup time. There is no auto-detection for this parameters and a wrong value will not let webdar be able to decipher the backup"
#define TOOLTIP_HOR_SIG_FAILURE "If the backup has been encrypted *and* signed with a GPG key pair, the signature will be checked at reading time. If it does not validate, webdar will abort unless this control is disabled"
#define TOOLTIP_HOR_EXECUTE "Run a shell command before reading each slice"
#define TOOLTIP_HOR_MIN_DIGITS "Webdar will detected this value if choosing an backup to read using the file selection window (which opens clicking on the \"+\" button). Though, if you manually fill the fields of the archive path and base name, you will have to set this value manually also for webdar be able to read the backup"
#define TOOLTIP_HOR_INFO_DETAILS TOOLTIP_COMMON_INFO_DETAILS
#define TOOLTIP_HOR_LAX "In case of archive corruption, activating the lax mode is the last resort solution to try reading something out of the backup. This mode suppresses a lot of checks and need is very chatty as it offers a lot of options to the user to bypass or modify the parameters read from the archive format."
#define TOOLTIP_HOR_SEQ_READ "Setting sequential read mode leads to a slower process than the normal (direct access) mode. But it is the only mode available when reading an archive directly from tapes, for example. It is also the only way to read a truncated backup, something that occurs when you have consumed all the free space of disk during a backup process, and do not have at least one slices in addition to the current one webdar is writing to, to move away in order free some spaace and let Webdar continue the backup process"
#define TOOLTIP_HOR_FORCE_FIRST_SLICE "When an external catalog is used (see the section below), Webdar fetches the backup content from the external catalog, but still needs to read the archive format version from the backup itslef. This information is located at two places: the beginning of the first slice and the end of the last slice. In sequential read mode, there is no choice, because the archive is read sequentially and this information is needed very early. In direct access mode (sequential mode unchecked) this information is fetched by default at the end of the last slice. But some users having huge backups or archives (several terabytes) configure their backup with a very small first slice (one kilobyte for example) while other slices can hold several gigabytes. Reading the first slice avoids fetching the last slice which may be huge and take time just for that, so they prefer loading the small first one which is fast and then only load the big slices where the data to restore is located"
#define TOOLTIP_HOR_THREADS_COMPR "If compression has not be configured per block, only one thread will be created and used to decompress each file's data"
#define TOOLTIP_HOR_EXTERNAL_CAT "By default the table of content is found inside the backup. However you may isolate it as an external backup, which only contains the table of content (the catalog) and no data. This can be used as backup of the internal catalog in case of corruption or to optimize some operations when manipulating very big backups or archives, see the \"Isolate\" menu on the right"

    // html_slicing class

#define TOOLTIP_HS_SLICING "A backup or archive can be stored is several file a fixed size (caled sliced). Slices cannot be used alone but may not be all needed depending on the requested operation, this also overcome some filesize limitation of some filesystems"
#define TOOLTIP_HS_SLICE_SIZE "Maximum size a slice can have, the last slice will be smaller most of the time"
#define TOOLTIP_HS_FIRST_SLICE "Choose a different slice size (bigger or smaller) for the first slice"
#define TOOLTIP_HS_FIRST_SLICE_SIZE "Specific size of the first slice, It may be smaller if the whole backup or archive could be hold in one slice"
#define TOOLTIP_HS_PERMISSION "By default the slice permission is readable and writable by all and restricted by the umask (which usually restricts the write access to the slice owner), you can use 0600 for slices to only be accessible to the slice owner"
#define TOOLTIP_HS_USER "User ownership to set for slices, this assumes webdar has the right to set file ownership, if not or if undefined here, the user ownership will be the user of the webdar process"
#define TOOLTIP_HS_GROUP "Group ownership to set for slices, this assumes webdar has the right to set file ownership, if not or if undefined here, the group ownership will be the group of the webdar process"
#define TOOLTIP_HS_MIN_DIGITS "From a given <basename>, webdar creates slices adding a number (the slice number) followed by the \"dar\" extension, like \"<basename>.1.dar\". If you have many slices, you may like to have small slice number prefixed with zeros. Indicating 4 as min-digits will lead slices of that form \"<basename>.0001.dar\""

    // saisie class

#define TOOLTIP_S_SESSION "You can change the name of the session to better identify it among others. See the \"Other sessions\" menu on the left"

    // html_options_test class

#define TOOLTIP_HOT_EMPTY "If checked, the archive content is read but not file's data is read and checked against its CRC data, leading to a quick process but not performing a full backup testing"
#define TOOLTIP_HOT_INFO_DETAILS TOOLTIP_COMMON_INFO_DETAILS
#define TOOLTIP_HOT_DISPLAY_TREATED TOOLTIP_COMMON_DISPLAY_TREATED
#define TOOLTIP_HOT_DISPLAY_ONLY_DIR TOOLTIP_COMMON_DISPLAY_ONLY_DIR
#define TOOLTIP_HOT_DISPLAY_SKIPPED TOOLTIP_COMMON_DISPLAY_SKIPPED


#endif
