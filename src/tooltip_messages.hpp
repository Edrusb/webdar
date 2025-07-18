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

    // webdar headers

    // common messages

#define TOOLTIP_COMMON_INFO_DETAILS "Show information about the archive layer creation, slicing generation, but nothing about the treated files and directories"
#define TOOLTIP_COMMON_DISPLAY_SKIPPED "Display files that have been excluded from the operation. If a directory is excluded from the backup, all its content is excluded and nothing is reported further about that directory and its content"
#define TOOLTIP_COMMON_DISPLAY_TREATED "Display information about files an directories while they are treated"
#define TOOLTIP_COMMON_DISPLAY_ONLY_DIR "Restrict the information about the treated directories only, a line is displayed when the backup process enters each directory"
#define TOOLTIP_COMMON_FURTIVE_READ "Reading a file modifies the last access time, or if after reading it, the last access time (atime) is set back to the value it has before the file was read, the system changes the last change time (ctime) which cannot be modified from an application, like Webdar. Linux system provides a privileged mode to access a file without modifying neither atime nor ctime. Webdar must be root or have the proper capability set to activate this mode. If activation failed, Webdar will default to the mode you chose (modifying atime is the most common setting)"
#define TOOLTIP_COMMON_ZEROING_DATE "Some file systems support negative dates, which means dates before year 1970. Webdar cannot handle such negative dates, but can consider these as zero, which corresponds to the end of the last second of year 1969"
#define TOOLTIP_COMMON_HOURSHIFT "On some systems (mainly under Windows) the daylight saving time is performed by modifying file dates, not just the way the clock is displayed on the system. This leads to save files that have not changed when passing to or from summer time. If set to 1 or more, a file will not be considered to have changed if only its date changed and by an exact number of hours"
#define TOOLTIP_COMMON_WTC "How to consider a file has changed"
#define TOOLTIP_COMMON_EMPTY "Perform the operation but send the generated backup bytes into black hole, leading no backup to be created at all, though you can get messages from the file system reading process and Webdar filter mechanisms"
#define TOOLTIP_COMMON_ALLOW_OVER "If unchecked, this prevents Webdar to overwrite slices of a previous backup having the same name, else unless disabled below, a warning and confirmation request is issued before overwriting"
#define TOOLTIP_COMMON_WARN_OVER "Warn and ask for confirmation before overwriting slices of an existing backup using the same name as the one we are about to create"
#define TOOLTIP_COMMON_PAUSE "Pause Webdar after each 'N' slice, a confirmation is then asked to continue the operation for the next N slices before pausing again. The objective of that feature was to treat the generated slices like transferring them through the network or at earlier time to burn it or them to a CD or DVD disc"
#define TOOLTIP_COMMON_USER_COMMENT "This field is available for the user to add any information but pay attention, this information is stored in clear in the backup header even if the backup is ciphered"
#define TOOLTIP_COMMON_HASH_ALGO "With this option a hash of the generated slices is computed in memory during the slice creation and written to a separated file beside the slice to let you check that no data alteration came on disk. Use the -c option of md5sum, sha1sum, sh1512sum or rhash -W command to easily check those hashes. For example: \"rhash -W -c slice.1.dar.whirlpool\""
#define TOOLTIP_COMMON_EXECUTE "Execute a shell command after each slice completion"
#define TOOLTIP_COMMON_EMPTY_DIR "When a directory is excluded from a backup, as expected it does not show in the backup and is thus not restored. When this concerns a mount point for virtual file systems (like /proc, /sys, /dev and mounted tmpfs often found for /tmp and /run), this option let Webdar store any excluded directory as an empty directory, including those mount points you'd be advise not to backup.  Then, at restoration time, Webdar will automatically recreate the mount points for those virtual file systems you have excluded from your backup"
#define TOOLTIP_COMMON_SEQ_MARKS "You should not disable sequential marks unless you know what you are doing. These marks (sometime also called tape marks) provides a redundant method to read a backup, in particular in case of file system saturation during a backup process; they may also help recover a backup that has been corrupted on disk or tape"
#define TOOLTIP_COMMON_SPARSE_MIN "Long sequences of zeroed bytes in a file may be stored as a \"hole\" on the file system, leading to no or very little disk space consumption, while such file can span several terabytes, for example. Webdar can detect those holes and record them as a short data structures in the backup. Even without compression, this saved a lot of space in the resulting backup and let Webdar recreate the so called holes at restoration time avoiding creating a huge file when it can leverage the underlying file system ability to handle sparse files. It does not make sense to set a value below 15 bytes, but you can disable this feature setting it to zero, this will increase the backup speed at the cost of the inability to store an restore sparse files"
#define TOOLTIP_COMMON_THREADS_COMPR "The number of threads for data (de)compression. Only one thread will be created and used when not using block compression"
#define TOOLTIP_COMMON_IN_PLACE "At backup time, Webdar records directory used for root of the backup in the archive. Checking this option ignore the specified file system root and use this recorded \"in-place\" path from the backup instead of what is specified as root"
#define TOOLTIP_COMMON_BASENAME "A backup is composed of one or more slices (See slicing section in the options below). These files will be named after this basename followed by a slice number and a \"dar\" extension, for example: \"mybasename.1.dar\""
#define TOOLTIP_COMMON_SAUV_PATH "Location where will be written the backup, see also the options below to define the repository (the host) where this path is located"
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
#define TOOLTIP_HC_ALGO "Ciphering algorithm, most of the time you will want to use AES"
#define TOOLTIP_HC_PASS1 "The password or passphrase to use to cipher the backup (and to remember in order to decipher it)"
#define TOOLTIP_HC_PASS2 "Retype the same password or passphrase as entered above"
#define TOOLTIP_HC_SIZE "Keep this parameter to its default value (10 kio), else you will have to remember its value and set it back at the same value in order to decipher the backup"
#define TOOLTIP_HC_THREADS "Number of threads to use to cipher or decipher the data"
#define TOOLTIP_HC_FS_KDF_HASH "Humans choose password/phrase in a very restrictive name space. The Key Derivation Function, takes this pass and a salt (an arbitrarily chosen sequence stored in clear in the archive header) as input of a time consuming transformation process to obtain a more randomized encryption key. With the salt, different archives use different encryption keys even if the human provided password is the same, while the CPU intensive transformation makes dictionary attack more costly"
#define TOOLTIP_HC_KDF_HASH0 "md5 is not a strong hash algorithm and it outputs in a small name space. Avoid using it for KDF, it is here to support old archive formats"
#define TOOLTIP_HC_KDF_HASH1 "sha1 is also here to support old archive format, don't use it anymore"
#define TOOLTIP_HC_KDF_HASH2 "sha512 can be used with a high number of iteration for the time to get the ciphering key from salt and pass to take a few seconds on an average computer"
#define TOOLTIP_HC_KDF_HASH3 "whirlpool has a very large output name space, though use it with enough iteration for the KDF process to take also several seconds on an average computer"
#define TOOLTIP_HC_KDF_HASH4 "argon2 is known as the reference for KDF function, this is the default and best choice according to literature since 2015 and at least up to year 2023"
#define TOOLTIP_HC_ITERATION_COUNT "The hash result of the salt and pass is hashed several times in loop, for the process of getting a randomized string outside the human usual name space to cost CPU resource and time and drive an dictionary attack slow and/or costly in time and or CPU"

    // html_compression_params class

#define TOOLTIP_HCP_COMPR "The algorithms differ in execution time to compress and to decompress, as well as the average compression ratio obtained depending on the nature of the data to compress, choose the algorithm that best fits your use case,"
#define TOOLTIP_HCP_COMPR_LEVEL "The higher this number is, the more CPU and time will be spend to reach the best compression ratio and lower the storage footprint"
#define TOOLTIP_HCP_MIN_COMPR "Files under that size will never be compressed"
#define TOOLTIP_HCP_COMPR_BLOCK "If set to zero, use stream compression which gives the best compression result but cannot use several threads. Else, split the data to compress in given block size and compress those block independently, something that can be spread between different threads, though it increases the memory and CPU footprint, at most by the number of threads set: small files may not mobilize all threads if the number of blocks they contain is less than the number of available threads"
#define TOOLTIP_HCP_NEVER_RESAVE "When compressed data of a file uses more space than uncompressed, by default it is stored uncompressed, which means re-reading it again and overwriting the compressed data written to the backup. If checked, keep data compressed in any case"
#define TOOLTIP_HCP_THREADS TOOLTIP_COMMON_THREADS_COMPR
#define TOOLTIP_HCP_KEEP_COMPR "For merging operation only, do not try to decompress and re-compress files that are retained in the resulting archive. If checked, the current configuration will be refused for other operations, like backup or isolation"

    // html_demo class

#define TOOLTIP_D_LEFT0 "the box above is hidden from the user, but its value is kept recorded"
#define TOOLTIP_D_LEFT1 "the box above is visible but cannot be modified by the user"
#define TOOLTIP_D_LEFT2 "the box is editable and visible (which most of the time is the default setting)"
#define TOOLTIP_D_LI "enter text here"
#define TOOLTIP_D_RI "this is the information known by Webdar at this point in time"
#define TOOLTIP_D_COUNTER "This shows the provided string size as computed at the time the \"Calculate\" button has last been pressed"
#define TOOLTIP_D_LABEL "This is how would show additional information for user help and feature detailed description"

    // html_entrepot class

#define TOOLTIP_HE_REPO_TYPE "type \"local\" is where Webdar is running, other type should point to an existing and reachable server from the node where Webdar is running"
#define TOOLTIP_HE_HOST "hostname or IP address"
#define TOOLTIP_HE_PORT "TCP port the service listens on"
#define TOOLTIP_HE_LOGIN "login name to connect to the remote server"
#define TOOLTIP_HE_PASS "password to use to authenticate the login user on that remote server"
#define TOOLTIP_HE_AUTH "To authenticate using a keyfile, provided a key pair, which public key has been added to the \"~/.ssh/authorized_keys\" for that login account on that remote server. See ssh documentation for more info"
#define TOOLTIP_HE_PUB "This file should contain the public key part of the key pair"
#define TOOLTIP_HE_PRV "This file should contain the private key counterpart of the public key file"
#define TOOLTIP_HE_KNOWN_CHECK "To avoid connecting to a pirate server, ssh record the server key fingerprint during the first connection and store them in a file named known_hosts. Upon a new connection to to a know server, if the server fingerprint has changed ssh refuse to connect and let you send your credentials and transfer data to a potential wrong server. You can disable this check assuming you know what you are doing!"
#define TOOLTIP_HE_KNOWN_FILE "It is possible change from the default location of the known_hosts file, the common location is provided here by default"
#define TOOLTIP_HE_WAIT "Webdar does not stop upon a network issue, like a lack of connectivity or disconnection from the remote server, it waits this amount of seconds and retry until it succeeds or is interrupted by the Webdar user. Choosing too short delay may overload the remote server and defeat the delay feature that is here to let the server address overload and proceed later when more resources are available"
#define TOOLTIP_HE_VERBOSE "For troubleshooting purpose, verbose information can be displayed on command-line interface where Webdar has been launched from"

    // html_entrepot_landing class

#define TOOLTIP_HEL_USE "If checked, when this repository is selected for an operation, the repository path is replaced by the landing path value provided here"
#define TOOLTIP_HEL_PATH "A landing path is a path on the remote server where you want Webdar to change to, at connection time. It is expected to be the place where, by default, you store your backups on that server. It will be used by default when no path has yet been set for an operation"

    // html_form_mask_expression class

#define TOOLTIP_HFME_TYPE "Glob expression is expression you commonly have under a shell with special characters like * ? and []"
#define TOOLTIP_HFME_NEGATE "When a file matches the mask consider it does not and vice versa"
#define TOOLTIP_HFME_EXPRESSION "Expression, eventually with wildcards (pay attention to the mask type above) that a file must satisfy to be considered in the operation"

    // html_form_mask_file class

#define TOOLTIP_HFMF_FILENAME "filename containing a list of path+filename to include or exclude from the operation"
#define TOOLTIP_HFMF_EXCLUDE "If checked, entries listed in the provided are excluded from the operation, else there are included into the operation"
#define TOOLTIP_HFMF_CASESENSIT "Consider case sensitivity or not for the file listing content in the provided filename"

    // html_form_mask_subdir class

#define TOOLTIP_HFMS_TYPE "Pay attention, only excluding mask support mask (glob and regex), in included path no character has a special meaning"
#define TOOLTIP_HFMS_CASE "case sensitivity when comparing this mask with file to process"
#define TOOLTIP_HFMS_REGEX "If unchecked the provided mask is assumed a glob expression, else if checked it is assumed a regular expression"
#define TOOLTIP_HFMS_MASK "sub-directory to match against. Pay attention, absolute path are not allowed in all context, the use of such configuration will not be accepted in that cases"

    // html_form_overwrite_action class

#define TOOLTIP_HFOA_TYPE "Select the type of action and click the update button"

    // html_form_overwritre_base_criterium class
#define TOOLTIP_HFOBC_NEGATE "Take the opposite result of the selected criterion, for example if the selected criterion is \"in place is an inode\" negating it will result in \"in place is *not* an inode\""
#define TOOLTIP_HFOBC_INVERT "Invert the roles of the \"in-place\" entry and the \"to be added\" one, for example if the selected criterion is \"in-place is an inode\" inverting will result in \"*to be added* is an inode\""
#define TOOLTIP_HFOBC_BASE "Define what action to do when the same entry is found in the \"in-place\" archive and in the \"to be added\" one"


    // html_form_overwrite_chain_action class

#define TOOLTIP_HFOCA_FS "In a chain of action, evaluations are performed one after the other; if a decision is set for Data or for EA it is not changed by subsequent action, the use of the \"keep undefined\" decision will be of most use inside the first cells of a chain of action"

    // html_form_overwrite_constant_action class

#define TOOLTIP_HFOCA_DATA "Defines the overwriting policy for the data, file's data, pointed to file of a symlink, major and minor of a char or block device, it also concerns permissions, ownership and dates of any inode type"
#define TOOLTIP_HFOCA_EA "Defines the overwriting policy for the Extended Attributes (EA) and File Specific Attributes (FSA) of any inode type"

    // html_form_sig_block_size

#define TOOLTIP_HFSB_DELTA "Better not calculating delta signature but re-saving as a whole small files upon change, this saves both CPU cycles and storage requirements"
#define TOOLTIP_HFSB_FUNCTION "Delta signature is what is used to compare with the previous status of a file. It is calculated per block of a file's data. The larger the block is, the less blocks will be, resulting in a smaller delta signature for a given file size, but the more data will have to be re-saved upon binary change inside that file, as the block size is bigger"
#define TOOLTIP_HFSB_FUNCTION0 "Block size is independent from the file sizes, consider adjusting the multiplicative and/or divisor factors below. Delta signature will grow proportionally to the file size"
#define TOOLTIP_HFSB_FUNCTION1 "Block size increases very slowly when the file size increases. This bring some advantage compare to the linear function to reduce the delta signature size for big files without much impact on the amount of data to re-save upon change"
#define TOOLTIP_HFSB_FUNCTION2 "Block size increases quicker than logarithmic based function, when the file size increases"
#define TOOLTIP_HFSB_FUNCTION3 "Block size increases even quicker when the file size increases, but less faster than the linear function"
#define TOOLTIP_HFSB_FUNCTION4 "Block size increase proportionally to the size of the file, consider setting the divisor factor to a value high enough, for example greater than 10, to avoid having too few or even a single block per file. Binary delta signature should almost have the same size whatever is the file size"
#define TOOLTIP_HFSB_MULTIPLY "Adjust the formula by multiplying it by a fixed factor. Click the \"Update\" button and look at the new formula expression above"
#define TOOLTIP_HFSB_DIVISOR "Adjust the formula by dividing it with a fixed factor. Can be combined with the multiplicative factor too"
#define TOOLTIP_HFSB_MIN_SIZE "Bypass the result of the formula described above for the delta signature block size not to be smaller than this provided value"
#define TOOLTIP_HFSB_MAX_SIZE "Bypass the result of the formula described above for the delta signature block size not to be larger than this provided value"

    // html_fsa_scope class

#define TOOLTIP_HFS_FS "Some attributes are specific to some file systems, like the immutable flag for Linux ext2/3/4 or the birth or creation date on MacOS HFS+ file system. Webdar can save and restore them, and when supported by the file system. However if you restore across different file system types, you may get a lot of warnings. You have the option to ignore them at restoration time or, here, to directly avoid backing them up"

    // html_options_create class

#define TOOLTIP_HOC_ARCHTYPE0 "Save all files and directories under the different file and path based filters, perimeters, etc. as defined in the following sections"
#define TOOLTIP_HOC_ARCHTYPE1 "Only saves files that have changed since the backup given as reference has been made. This backup of reference may be a full backup or another incremental/differential backup as the one about to create here, with that option set. Of course here too all file and path based filters, perimeters, etc. also apply on top"
#define TOOLTIP_HOC_ARCHTYPE2 "Same as previously, with the addition to not save the modified files in totality, but leverage delta signatures to only record the portions of the files that changed. Note: at restoration time you will need to restore the files from the previous backup then restore this backup on top to apply a binary patch on files saved that way"
#define TOOLTIP_HOC_ARCHTYPE3 "Make a reference of the current file system status without saving any file. A snapshot can be used as a reference for a differential backup or just be used to check whether existing files at the time of the snapshot have changed since the snapshot was made"
#define TOOLTIP_HOC_ARCHTYPE4 "Only change files modified after a given date. This creates a differential backup without requiring a backup of reference"
#define TOOLTIP_HOC_WTC TOOLTIP_COMMON_WTC
#define TOOLTIP_HOC_HOURSHIFT TOOLTIP_COMMON_HOURSHIFT
#define TOOLTIP_HOC_BIN_DELTA "To add delta signature to a backup, see below the \"Delta Signatures\" section"
#define TOOLTIP_HOC_FURTIVE_READ TOOLTIP_COMMON_FURTIVE_READ
#define TOOLTIP_HOC_ZEROING_DATE TOOLTIP_COMMON_ZEROING_DATE
#define TOOLTIP_HOC_MOD_DATA_DETECT "In the context of differential/incremental backup, normally the data of a file is considered to have changed if the mtime (last modification time) has changed. In libdar older versions, a file was completely re-saved if any date, permission or ownership of a file had changed. Since libdar 2.6.0 those metadata change (permission, ownership, atime, ctime...) can be saved without having to re-save the whole file content, this reduces a lot of the backup size and shorten the backup process"
#define TOOLTIP_HOC_ALLOW_OVER TOOLTIP_COMMON_ALLOW_OVER
#define TOOLTIP_HOC_WARN_OVER TOOLTIP_COMMON_WARN_OVER
#define TOOLTIP_HOC_PAUSE TOOLTIP_COMMON_PAUSE
#define TOOLTIP_HOC_RETRY_TIMES "Right before saving a file, Webdar record its last modification date, if right after the backup of that file this date has changed, Webdar retries to save it again and so forth if the date changed again, but for a limited amount of times. Passed that, the file is flagged as \"dirty\" meaning the file content may be inconsistent regarding what a user application may expect to find in it. At restoration time you will have different options on how to treat dirty files"
#define TOOLTIP_HOC_RETRY_OVER "Re-saving a changed file does not consume additional data in normal condition. This is not the case when saving to a pipe where it is not possible to overwrite the new version of the file in the generated backup. This parameter let define a maximum amount of bytes that are wasted that way during the overall backup process, after which no data retry is performed and files are directly flagged as dirty if they changed while they were read for backup"
#define TOOLTIP_HOC_SEQ_MARKS TOOLTIP_COMMON_SEQ_MARKS
#define TOOLTIP_HOC_SPARSE_MIN TOOLTIP_COMMON_SPARSE_MIN
#define TOOLTIP_HOC_USER_COMMENT TOOLTIP_COMMON_USER_COMMENT
#define TOOLTIP_HOC_HASH_ALGO TOOLTIP_COMMON_HASH_ALGO
#define TOOLTIP_HOC_EXECUTE TOOLTIP_COMMON_EXECUTE
#define TOOLTIP_HOC_EMPTY TOOLTIP_COMMON_EMPTY
#define TOOLTIP_HOC_INFO_DETAILS TOOLTIP_COMMON_INFO_DETAILS
#define TOOLTIP_HOC_DISPLAY_TREATED TOOLTIP_COMMON_DISPLAY_TREATED
#define TOOLTIP_HOC_DISPLAY_ONLY_DIR TOOLTIP_COMMON_DISPLAY_ONLY_DIR
#define TOOLTIP_HOC_DISPLAY_SKIPPED TOOLTIP_COMMON_DISPLAY_SKIPPED
#define TOOLTIP_HOC_DISPLAY_DIR_SUMMARY "Display a summary line about the amount of saved data and its compression ratio at the end of each treated directory"
#define TOOLTIP_HOC_SECU_CHECK "When a file has only changed by its ctime since the backup of reference was made, with that option Webdar triggers a security warning. This may be expected when for example changing a file permission and setting it back to the previous value, it may also be a sign of concealment when one tries to hide changes made to a file. If such warnings occur too often, use an anti-virus or rootkit scanner and disable this feature in Webdar"
#define TOOLTIP_HOC_IGNORE_UNKNOWN "When Webdar finds an inode type it does not know, a warning is issued. You can disable this warning by checking this option, no warning will show about those unknown inode type and the backup process will complete without considering them at all"
#define TOOLTIP_HOC_EMPTY_DIR TOOLTIP_COMMON_EMPTY_DIR
#define TOOLTIP_HOC_CACHE_TAG "Many applications use temporary data on disk for caching purposes and flag those directories following the cache directory tagging standard. Webdar understands that and can avoid saving directories tagged that way, when this option is checked"
#define TOOLTIP_HOC_NODUMP "Some file system let users and applications set a 'nodump' flag on files, meaning they should not be backed up. Webdar can exclude those files from a backup, if this option is checked"
#define TOOLTIP_HOC_EXCLUDE_BY_EA "In complement to the nodump and cache directory tagging way to exclude files, Webdar proposes the use of a well-defined Extended Attribute to flag files and directories to be exclude from the backup"
#define TOOLTIP_HOC_DEFAULT_EA "Use the default Extended Attribute (if this option is set, its value shows below in the disabled box)"
#define TOOLTIP_HOC_EXCLUDE_BY_EA_NAME "Specify here the EA name of your choice and flag files accordingly with that EA for Webdar not to save them"
#define TOOLTIP_HOC_SAME_FS "The whole file system where the below specified directories reside will be either include or excluded from the backup. if only included file system are specified, none of the other file system will be looked at for backup, though the file system where is located the path defined as \"file system root\" for a backup, is never excluded. If only excluded directories are specified, any other file system will be considered for backup (applying other filtering mechanisms you have set). If there is a mix of included and excluded file systems only included file system that have not also been specified to be excluded will be considered for the backup operation"

    // html_options_read class

#define TOOLTIP_HOR_ALGO "Unless reading a very old archive, use \"none\" here even if the backup is ciphered, algorithm is auto-detected"
#define TOOLTIP_HOR_PASS "If not specified here, password/passphrase will be asked later. Pay attention that setting password here will lead it to be saved unencrypted in Webdar configuration file"
#define TOOLTIP_HOR_SIZE "keep the default 10 kio unless you have changed it at backup time. There is no auto-detection for this parameters and a wrong value will not let Webdar be able to decipher the backup"
#define TOOLTIP_HOR_SIG_FAILURE "If the backup has been encrypted *and* signed with a GPG key pair, the signature will be checked at reading time. If it does not validate, Webdar will abort unless this control is disabled"
#define TOOLTIP_HOR_EXECUTE "Run a shell command before reading each slice"
#define TOOLTIP_HOR_MIN_DIGITS "Webdar will detected this value if choosing an backup to read using the file selection window (which opens clicking on the \"+\" button). Though, if you manually fill the fields of the archive path and base name, you will have to set this value manually also for Webdar be able to read the backup"
#define TOOLTIP_HOR_INFO_DETAILS TOOLTIP_COMMON_INFO_DETAILS
#define TOOLTIP_HOR_LAX "In case of archive corruption, activating the lax mode is the last resort solution to try reading something out of the backup. This mode suppresses a lot of checks and need is very chatty as it offers a lot of options to the user to bypass or modify the parameters read from the archive format."
#define TOOLTIP_HOR_SEQ_READ "Setting sequential read mode leads to a slower process than the normal (direct access) mode. But it is the only mode available when reading an archive directly from tapes, for example. It is also the only way to read a truncated backup, something that occurs when you have consumed all the free space of disk during a backup process, and do not have at least one slices in addition to the current one Webdar is writing to, to move away in order free some space and let Webdar continue the backup process"
#define TOOLTIP_HOR_FORCE_FIRST_SLICE "When an external catalog is used (see the section below), Webdar fetches the backup content from the external catalog, but still needs to read the archive format version from the backup itself. This information is located at two places: the beginning of the first slice and the end of the last slice. In sequential read mode, there is no choice, because the archive is read sequentially and this information is needed very early. In direct access mode (sequential mode unchecked) this information is fetched by default at the end of the last slice. But some users having huge backups or archives (several terabytes) configure their backup with a very small first slice (one kilobyte for example) while other slices can hold several gigabytes. Reading the first slice avoids fetching the last slice which may be huge and take time just for that, so they prefer loading the small first one which is fast and then only load the big slices where the data to restore is located"
#define TOOLTIP_HOR_THREADS_COMPR TOOLTIP_COMMON_THREADS_COMPR
#define TOOLTIP_HOR_EXTERNAL_CAT "By default the table of content is found inside the backup. However you may isolate it as an external backup, which only contains the table of content (the catalog) and no data. This can be used as backup of the internal catalog in case of corruption or to optimize some operations when manipulating very big backups or archives, see the \"Isolate\" menu on the right"

    // html_slicing class

#define TOOLTIP_HS_SLICING "A backup or archive can be stored is several file a fixed size (called sliced). Slices cannot be used alone but may not be all needed depending on the requested operation, this also overcome some file size limitation of some file systems"
#define TOOLTIP_HS_SLICE_SIZE "Maximum size a slice can have, the last slice will be smaller most of the time"
#define TOOLTIP_HS_FIRST_SLICE "Choose a different slice size (bigger or smaller) for the first slice"
#define TOOLTIP_HS_FIRST_SLICE_SIZE "Specific size of the first slice, It may be smaller if the whole backup or archive could be hold in one slice"
#define TOOLTIP_HS_PERMISSION "By default the slice permission is readable and writable by all and restricted by the umask (which usually restricts the write access to the slice owner), you can use 0600 for slices to only be accessible to the slice owner"
#define TOOLTIP_HS_USER "User ownership to set for slices, this assumes Webdar has the right to set file ownership, if not or if undefined here, the user ownership will be the user of the Webdar process"
#define TOOLTIP_HS_GROUP "Group ownership to set for slices, this assumes Webdar has the right to set file ownership, if not or if undefined here, the group ownership will be the group of the Webdar process"
#define TOOLTIP_HS_MIN_DIGITS "From a given <basename>, Webdar creates slices adding a number (the slice number) followed by the \"dar\" extension, like \"<basename>.1.dar\". If you have many slices, you may like to have small slice number prefixed with zeros. Indicating 4 as min-digits will lead slices of that form \"<basename>.0001.dar\""

    // saisie class

#define TOOLTIP_S_SESSION "You can change the name of the session to better identify it among others. See the \"Other sessions\" menu on the left"

    // html_options_test class

#define TOOLTIP_HOT_EMPTY "If checked, the archive content is read but not file's data is read and checked against its CRC data, leading to a quick process but not performing a full backup testing"
#define TOOLTIP_HOT_INFO_DETAILS TOOLTIP_COMMON_INFO_DETAILS
#define TOOLTIP_HOT_DISPLAY_TREATED TOOLTIP_COMMON_DISPLAY_TREATED
#define TOOLTIP_HOT_DISPLAY_ONLY_DIR TOOLTIP_COMMON_DISPLAY_ONLY_DIR
#define TOOLTIP_HOT_DISPLAY_SKIPPED TOOLTIP_COMMON_DISPLAY_SKIPPED

    // html_options_compare class

#define TOOLTIP_HOC_FURTIVE TOOLTIP_COMMON_FURTIVE_READ
#define TOOLTIP_HOC_ZEROING_DATES TOOLTIP_COMMON_ZEROING_DATE
#define TOOLTIP_HOC_HOURSHIFT TOOLTIP_COMMON_HOURSHIFT
#define TOOLTIP_HOC_IN_PLACE TOOLTIP_COMMON_IN_PLACE
#define TOOLTIP_HOC_WTC TOOLTIP_COMMON_WTC
#define TOOLTIP_HOC_SYMLINK_DATE "If checked, Webdar will signal a change if the symlink last modification date has changed, even if the object pointed to by the symlink has not changed at all"
#define TOOLTIP_HOC_INFO_DETAILS TOOLTIP_COMMON_INFO_DETAILS
#define TOOLTIP_HOC_DISPLAY_TREATED TOOLTIP_COMMON_DISPLAY_TREATED
#define TOOLTIP_HOC_DISPLAY_ONLY_DIR TOOLTIP_COMMON_DISPLAY_ONLY_DIR
#define TOOLTIP_HOC_DISPLAY_SKIPPED TOOLTIP_COMMON_DISPLAY_SKIPPED

    // html_options_isolate class

#define TOOLTIP_HOI_DELTA_SIG "By default delta signature if present are not stored in a isolated catalog. Check this box if you want to make a differential backup with delta signatures using this catalog as reference"
#define TOOLTIP_HOI_DELTA_TRANSFER "If the backup of reference lacks delta signatures it is possible in some circumstances to compute them (for file fully saved in the backup), check this box if you want to add some or drop some others delta signature from the backup of reference"
#define TOOLTIP_HOI_ALLOW_OVER TOOLTIP_COMMON_ALLOW_OVER
#define TOOLTIP_HOI_WARN_OVER TOOLTIP_COMMON_WARN_OVER
#define TOOLTIP_HOI_PAUSE TOOLTIP_COMMON_PAUSE
#define TOOLTIP_HOI_SEQ_MARKS "For an isolated catalog, sequential read marks does not bring much interest, though it does not hurt having them"
#define TOOLTIP_HOI_USER_COMMENT TOOLTIP_COMMON_USER_COMMENT
#define TOOLTIP_HOI_HASH_ALGO TOOLTIP_COMMON_HASH_ALGO
#define TOOLTIP_HOI_EXECUTE TOOLTIP_COMMON_EXECUTE
#define TOOLTIP_HOI_EMPTY TOOLTIP_COMMON_EMPTY
#define TOOLTIP_HOI_REPAIR_MODE "Check this box if the backup to fetch the catalog from has been truncated due to disk space saturation, Webdar will rebuild the catalog (table of content) from the inline sequential marks and to create this isolated catalog. This is an faster alternative to repairing the archive, which avoid recopying the whole data, but you will have to use this catalog as external catalog when reading the truncated backup"
#define TOOLTIP_HOI_INFO_DETAILS TOOLTIP_COMMON_INFO_DETAILS

    // html_options_merge class

#define TOOLTIP_HOM_ALLOW_OVER TOOLTIP_COMMON_ALLOW_OVER
#define TOOLTIP_HOM_WARN_OVER TOOLTIP_COMMON_WARN_OVER
#define TOOLTIP_HOM_PAUSE TOOLTIP_COMMON_PAUSE
#define TOOLTIP_HOM_SEQ_MARKS TOOLTIP_COMMON_SEQ_MARKS
#define TOOLTIP_HOM_SPARSE_MIN_SIZE TOOLTIP_COMMON_SPARSE_MIN
#define TOOLTIP_HOM_USER_COMMENT TOOLTIP_COMMON_USER_COMMENT
#define TOOLTIP_HOM_HASH_ALGO TOOLTIP_COMMON_HASH_ALGO
#define TOOLTIP_HOM_EXECUTE TOOLTIP_COMMON_EXECUTE
#define TOOLTIP_HOM_EMPTY TOOLTIP_COMMON_EMPTY
#define TOOLTIP_HOM_INFO_DETAILS TOOLTIP_COMMON_INFO_DETAILS
#define TOOLTIP_HOM_DISPLAY_TREATED TOOLTIP_COMMON_DISPLAY_TREATED
#define TOOLTIP_HOM_DISPLAY_ONLY_DIR TOOLTIP_COMMON_DISPLAY_ONLY_DIR
#define TOOLTIP_HOM_DISPLAY_SKIPPED TOOLTIP_COMMON_DISPLAY_SKIPPED
#define TOOLTIP_HOM_HAS_AUX "Merging operation takes a first mandatory backup in input (the backup of reference) and in option, a second backup, the auxiliary backup. If you wan not only filter out or change compression, encryption and slicing of a single backup, but merge the content of two backups, check this box"
#define TOOLTIP_HOM_DECREMENTAL "Decremental backup is a particular case of merging, where the backup of reference is an old full backup and the auxiliary backup is a recent backup, it result in creating a reverse differential backup, which, if restored after having restored the most recent backup provided here, will get to the state of the older full backup provided here too. This way you can replace old full backups by decremental ones (which should be smaller) and only keep the latest full backup"
#define TOOLTIP_HOM_DELTA_SIG "1/ No delta signature from a source backup will be transferred to the resulting backup of the merging operation.\n\n 2 - Any existing delta signature of a source backup (reference as well as the auxiliary and optional backup) will transferred beside the data of each file to the resulting backup\n\n 3 - This let you define which delta signatures you want to transfer and if possible compute (when the data is fully present for a file)"
#define TOOLTIP_HOM_EMPTY_DIR TOOLTIP_COMMON_EMPTY_DIR

    // html_options_repair class

#define TOOLTIP_HORP_ALLOW_OVER TOOLTIP_COMMON_ALLOW_OVER
#define TOOLTIP_HORP_WARN_OVER TOOLTIP_COMMON_WARN_OVER
#define TOOLTIP_HORP_PAUSE TOOLTIP_COMMON_PAUSE
#define TOOLTIP_HORP_EXECUTE TOOLTIP_COMMON_EXECUTE
#define TOOLTIP_HORP_EMPTY TOOLTIP_COMMON_EMPTY
#define TOOLTIP_HORP_THREADS_COMPR TOOLTIP_COMMON_THREADS_COMPR
#define TOOLTIP_HORP_HASH_ALGO TOOLTIP_COMMON_HASH_ALGO
#define TOOLTIP_HORP_INFO_DETAILS TOOLTIP_COMMON_INFO_DETAILS
#define TOOLTIP_HORP_DISPLAY_TREATED TOOLTIP_COMMON_DISPLAY_TREATED
#define TOOLTIP_HORP_DISPLAY_ONLY_DIR TOOLTIP_COMMON_DISPLAY_ONLY_DIR
#define TOOLTIP_HORP_DISPLAY_SKIPPED TOOLTIP_COMMON_DISPLAY_SKIPPED
#define TOOLTIP_HORP_USER_COMMENT TOOLTIP_COMMON_USER_COMMENT

    // html_options_extract class

#define TOOLTIP_HOE_IN_PLACE TOOLTIP_COMMON_IN_PLACE
#define TOOLTIP_HOE_WARN_OVER TOOLTIP_COMMON_WARN_OVER
#define TOOLTIP_HOE_WARN_NO_MATCH "A differential backup stores new and modified files but also record files that have been removed. At restoration time Webdar remove them, however if their inode type (symlink, plain file, directory...) does not match what is expected to be removed, this option triggers a warning for the user to decide what to do if that condition occurs"
#define TOOLTIP_HOE_FLAT "Do not restore directory tree, all files will be restored in the same directory, which may lead to overwriting files of the same name if they were located in different directories in the backup"
#define TOOLTIP_HOE_WTC TOOLTIP_COMMON_WTC
#define TOOLTIP_HOE_EMPTY_DIR TOOLTIP_COMMON_EMPTY_DIR
#define TOOLTIP_HOE_DIRTY "Dirty files are the one that have been modified at the time they were read for backup. Their content may be inconsistent for the applications using them, this option let you decide how to handle those files"
#define TOOLTIP_HOE_ONLY_DELETED "A differential backup stores new and modified files, but also record files that have been removed. With this option, no new or modified file will be restored, only file that should be removed will be deleted from the file system"
#define TOOLTIP_HOE_IGNORE_DELETED "A differential backup stores  new and modified files, but also record file that have been removed, for restoring a differential backup you update modified files, add new files and remove files that disappeared. If you check this option your restoration will be uncompleted and file that should have been removed will stay on the file system"
#define TOOLTIP_HOE_IGNORE_SOCKETS "Do no restore sockets, does not hurt restoring them, but usually applications will recreate them when restarted"
#define TOOLTIP_HOE_EMPTY TOOLTIP_COMMON_EMPTY
#define TOOLTIP_HOE_INFO_DETAILS TOOLTIP_COMMON_INFO_DETAILS
#define TOOLTIP_HOE_DISPLAY_TREATED TOOLTIP_COMMON_DISPLAY_TREATED
#define TOOLTIP_HOE_DISPLAY_ONLY_DIR TOOLTIP_COMMON_DISPLAY_ONLY_DIR
#define TOOLTIP_HOE_DISPLAY_SKIPPED TOOLTIP_COMMON_DISPLAY_SKIPPED

    // html_archive_create class

#define TOOLTIP_HAC_FS_ROOT "Nothing outside this directory will be considered for backup, all saved file will be stored in the backup with a path relative to this root directory"
#define TOOLTIP_HAC_SAUV_PATH TOOLTIP_COMMON_SAUV_PATH
#define TOOLTIP_HAC_BASENAME TOOLTIP_COMMON_BASENAME

    // html_archive_read class

#define TOOLTIP_HAR_ARCH_PATH "Path+basename of the backup to read. If you have several slices \"mybasename.1.dar\", \"mybasename.2.dar\" located in directory \"/tmp\" you should set this field to \"/tmp/mybasename\". Note that the \"+\" button at the end of this line will handle all these aspects including the min-digits option, you will only have to select on one of the slice of the backup you want to read"


    // html_archive_compare class

#define TOOLTIP_HACM_FS_ROOT "Path of the file system where is located the content to compare the backup with"

    // html_options_list class

#define TOOLTIP_HOL_ACTION0 "This option let you navigate in the directory tree of the backup and see for each directory its content and the metadata attributes of files they contain"
#define TOOLTIP_HOL_ACTION1 "This option gives a global view of the archive, the different options (compression, slicing, ciphering, user comment...) that have been used at creation time"

    // html_archive_isolate class

#define TOOLTIP_HAI_SAUV_PATH TOOLTIP_COMMON_SAUV_PATH
#define TOOLTIP_HAI_BASENAME TOOLTIP_COMMON_BASENAME

    // html_archive_merge class

#define TOOLTIP_HAM_SAUV_PATH TOOLTIP_COMMON_SAUV_PATH
#define TOOLTIP_HAM_BASENAME TOOLTIP_COMMON_BASENAME

    // html_archive_repair class

#define TOOLTIP_HAR_SAUV_PATH TOOLTIP_COMMON_SAUV_PATH
#define TOOLTIP_HAR_BASENAME TOOLTIP_COMMON_BASENAME

    // html_archive_extract class
#define TOOLTIP_HAE_FS_ROOT "Path were to restore the backup content"



#endif
