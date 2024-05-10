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

#ifndef TOKENS_HPP
#define TOKENS_HPP

    // C system header files
extern "C"
{

}
    // C++ system header files

    // WEBDAR Version
extern const char* WEBDAR_VERSION;

    // DAR archive extension
extern const char* EXTENSION;

    // the different answer codes
const unsigned int STATUS_CODE_CONTINUE = 101;
const unsigned int STATUS_CODE_SWITCHING_PROTOCOLS = 101;
const unsigned int STATUS_CODE_OK = 200;
const unsigned int STATUS_CODE_CREATED = 201;
const unsigned int STATUS_CODE_ACCEPTED = 202;
const unsigned int STATUS_CODE_NON_AUTHORITATIVE_INFORMATION = 203;
const unsigned int STATUS_CODE_NO_CONTENT = 204;
const unsigned int STATUS_CODE_RESET_CONTENT = 205;
const unsigned int STATUS_CODE_PARTIAL_CONTENT = 206;
const unsigned int STATUS_CODE_MULTIPLE_CHOICES = 300;
const unsigned int STATUS_CODE_MOVED_PERMANENTLY = 301;
const unsigned int STATUS_CODE_MOVED_TEMPORARILY = 302;
const unsigned int STATUS_CODE_NOT_MODIFIED = 304;
const unsigned int STATUS_CODE_USE_PROXY = 305;
const unsigned int STATUS_CODE_BAD_REQUEST = 400;
const unsigned int STATUS_CODE_UNAUTHORIZED = 401;
const unsigned int STATUS_CODE_FORBIDDEN = 403;
const unsigned int STATUS_CODE_NOT_FOUND = 404;
const unsigned int STATUS_CODE_METHOD_NOT_ALLOWED = 405;
const unsigned int STATUS_CODE_NOT_ACCEPTABLE = 406;
const unsigned int STATUS_CODE_PROXY_AUTHENTICATION_REQUIRED = 407;
const unsigned int STATUS_CODE_REQUEST_TIME_OUT = 408;
const unsigned int STATUS_CODE_CONFLICT = 409;
const unsigned int STATUS_CODE_GONE = 410;
const unsigned int STATUS_CODE_LENGTH_REQUIRED = 411;
const unsigned int STATUS_CODE_PRECONDITION_FAILED = 412;
const unsigned int STATUS_CODE_REQUEST_ENTITY_TOO_LARGE = 413;
const unsigned int STATUS_CODE_REQUEST_URI_TOO_LARGE = 414;
const unsigned int STATUS_CODE_UNSUPPORTED_MEDIA_TYPE = 415;
const unsigned int STATUS_CODE_INTERNAL_SERVER_ERROR = 500;
const unsigned int STATUS_CODE_NOT_IMPLEMENTED = 501;
const unsigned int STATUS_CODE_BAD_GATEWAY = 502;
const unsigned int STATUS_CODE_SERVICE_UNAVAILABLE = 503;
const unsigned int STATUS_CODE_GATEWAY_TIME_OUT = 504;
const unsigned int STATUS_CODE_HTTP_VERSION_NOT_SUPPORTED = 505;


    // HTTP headers

extern const char* HDR_CONTENT_LENGTH;
extern const char* HDR_IF_MODIFIED_SINCE;
extern const char* HDR_LAST_MODIFIED;
extern const char* HDR_CONTENT_TYPE;
extern const char* HDR_DATE;
extern const char* HDR_EXPIRES;
extern const char* HDR_SERVER;
extern const char* HDR_WWW_AUTHENTICATE;
extern const char* HDR_AUTHORIZATION;
extern const char* HDR_SET_COOKIE;
extern const char* HDR_COOKIE;
extern const char* HDR_LOCATION;

    // HTTP header values
extern const char* VAL_CONTENT_TYPE_FORM;


    // HTML CSS colors by fonction
extern const char* COLOR_BACK;
extern const char* COLOR_TEXT;
extern const char* COLOR_PADBACK;
extern const char* COLOR_PADFRONT;
extern const char* COLOR_PADBORD;
extern const char* COLOR_MENU_FRONT_ON;
extern const char* COLOR_MENU_BACK_ON;
extern const char* COLOR_MENU_BORDER_ON;
extern const char* COLOR_MENU_FRONT_OFF;
extern const char* COLOR_MENU_BACK_OFF;
extern const char* COLOR_MENU_BORDER_OFF;
extern const char* COLOR_MENU_FRONT_GREY;
extern const char* COLOR_MENU_BACK_GREY;
extern const char* COLOR_MENU_BORDER_GREY;
extern const char* COLOR_MENU_FRONT_HOVER_ON;
extern const char* COLOR_MENU_FRONT_HOVER_OFF;
extern const char* COLOR_MENU_FRONT_HOVER_GREY;
extern const char* COLOR_MENU_FRONT_ACTIVE_ON;
extern const char* COLOR_MENU_FRONT_ACTIVE_OFF;
extern const char* COLOR_MENU_FRONT_ACTIVE_GREY;

extern const char* COLOR_ARED; // almost red
extern const char* RED;
extern const char* WHITE;

extern const char* STATIC_PATH_ID;
extern const char* STATIC_OBJ_LICENSING;

#endif
