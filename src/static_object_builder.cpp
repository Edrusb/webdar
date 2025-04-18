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

#include "my_config.h"

extern "C"
{
#if HAVE_STDIO_H
#include <stdio.h>
#endif

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif

#if HAVE_ERRNO_H
#include <errno.h>
#endif
}

#include "base64.hpp"

#define CODE_OK 0
#define CODE_USAGE 1
#define CODE_ERROR 2
#define CODE_FILE_ERR 3

#define BUFSIZE 10240

int usage(char *cmd);
int error(char *type);
int routine(char *objname, char *type, char *arg);
int routine_text(char *objname, char *arg);
char *convert_file_to_text(char * filename);
unsigned int count_char_to_escape(int fd);
int escape_and_copy_from_file_to_string(int fd, char *buffer, unsigned int allocated);
int routine_jpeg(char *objname, char *arg);

using namespace std;

int main(int argc, char *argv[])
{
    try
    {
	if(argc != 4)
	    return usage(argv[0]);
	else
	    return routine(argv[1], argv[2], argv[3]);
    }
    catch(exception_base & e)
    {
	fprintf(stderr, "Aborting execution upon exception: %s\n", e.get_message().c_str());
	return CODE_ERROR;
    }
}

int usage(char *cmd)
{
    fprintf(stderr, "usage: %s <object_name> <type> <argument>\n", cmd);
    fprintf(stderr, "for type \"text\" the argument is the filename and path to the text file to build the object from\n");
    return CODE_USAGE;
}

int error(char *type)
{
    fprintf(stderr, "unknown type: %s\n", type);
    return CODE_ERROR;
}

int routine(char *objname, char *type, char *arg)
{
    if(strcmp(type, "text") == 0)
	return routine_text(objname, arg);
    else if(strcmp(type, "jpeg") == 0)
	return routine_jpeg(objname, arg);
    else
	return error(type);
}

int routine_text(char *objname, char *arg)
{
    char *chaine = convert_file_to_text(arg);

    if(chaine == NULL)
	return CODE_FILE_ERR;
    else
    {
	printf("static const char * text_%s = ", objname);
	printf("\"%s\";\n", chaine);
	free(chaine);
	chaine = NULL;
	printf("static_object_text *obj_%s = new (nothrow) static_object_text(text_%s);\n", objname, objname);
	printf("if(obj_%s == NULL)\n", objname);
	printf("    throw exception_memory();\n");
	printf("try\n{\n");
	printf("    add_object_to_library(%s, obj_%s);\n", objname, objname);
	printf("}\n");
	printf("catch(...)\n{\n");
	printf("    delete obj_%s;\n", objname);
	printf("    obj_%s = NULL;\n", objname);
	printf("}\n\n");
	return CODE_OK;
    }
}

char *convert_file_to_text(char * filename)
{
    char *ret = NULL;
    struct stat buf;

    if(stat(filename, & buf) != 0)
    {
	fprintf(stderr, "Error met while getting file size of %s: %s\n", filename, strerror(errno));
	return ret;
    }
    else
    {
	off_t size = buf.st_size;
	int fd = open(filename, O_RDONLY);

	if(fd < 0)
	{
	    fprintf(stderr, "Error met while reading file %s: %s\n", filename, strerror(errno));
	    return ret;
	}
	else
	{
		/* counting the number of char to escape */
	    size += count_char_to_escape(fd);
		/* adding space for the null terminator character */
	    size += 1;

	    if(lseek(fd, SEEK_SET, 0) == 0)
	    {
		ret = (char *)malloc(size);
		if(ret != NULL)
		{
		    unsigned int copied = escape_and_copy_from_file_to_string(fd, ret, size - 1);
		    if(copied < size - 1)
			fprintf(stderr, "Incoherence between file size and amount of available bytes in file %s\n", filename);
		    ret[copied] = '\0'; /* null terminator */
		}
	    }
	    else
		fprintf(stderr, "Failed seeking to the beginning of file %s\n", filename);

	    close(fd);
	}
    }

    return ret;
}

unsigned int count_char_to_escape(int fd)
{
    unsigned int count = 0;
    unsigned int i = 0;
    unsigned int lu = 0;
    char buffer[BUFSIZE];

    do
    {
	lu = read(fd, buffer, BUFSIZE);
	for(i = 0; i < lu; ++i)
	{
	    if(buffer[i] == '"'
	       || buffer[i] == '\\')
		++count;
	    if(buffer[i] == '\n')
		count += 3;
	}
    }
    while(lu > 0);

    return count;
}

int escape_and_copy_from_file_to_string(int fd, char *buffer, unsigned int allocated)
{
    unsigned int copied = 0;
    char tmp[BUFSIZE];
    ssize_t lu = 0;

    do
    {
	lu = read(fd, tmp, BUFSIZE);

	if(lu < 0)
	{
	    if(errno == EAGAIN)
		lu = 1; /* retrying the copy */
	    else
		lu = 0;
	}
	else
	{
	    unsigned int i = 0;

	    while(i < lu && copied < allocated)
	    {
		if(tmp[i] == '"' || tmp[i] == '\\')
		    buffer[copied++] = '\\';
		else
		    if(tmp[i] == '\n')
		    {
			buffer[copied++] = '\\';
			buffer[copied++] = 'n';
			buffer[copied++] = '\\';
		    }

		buffer[copied++] = tmp[i++];
	    }
	}
    }
    while(copied < allocated && lu > 0);

    return copied;
}

int routine_jpeg(char *objname, char *arg)
{
    int fd = open(arg, O_RDONLY);
    int ret;

    if(fd < 0)
    {
	cerr << "Error met while reading file " << arg << " : " << strerror(errno) << "endl";
	return CODE_FILE_ERR;
    }

    base64::decoded_block dec;
    base64::encoded_block enc;

    printf("static const char * jpeg_%s = \"", objname);

    do
    {
	ret = read(fd, dec, sizeof(dec));
	if(ret > 0)
	{
	    base64().small_encode(ret, dec, enc);
	    printf("%c%c%c%c", enc[0], enc[1], enc[2], enc[3]);
	}
    }
    while(ret > 0);

    printf("\";\n");
    printf("static_object_jpeg *obj_%s = new (nothrow) static_object_jpeg(jpeg_%s);\n", objname, objname);
    printf("if(obj_%s == NULL)\n", objname);
    printf("    throw exception_memory();\n");
    printf("try\n{\n");
    printf("    add_object_to_library(%s, obj_%s);\n", objname, objname);
    printf("}\n");
    printf("catch(...)\n{\n");
    printf("    delete obj_%s;\n", objname);
    printf("    obj_%s = NULL;\n", objname);
    printf("}\n\n");
    return CODE_OK;
}
