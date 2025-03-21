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

#include "tokens.hpp"

const char* EXTENSION = "dar";

const char* HDR_CONTENT_LENGTH = "Content-Length";
const char* HDR_IF_MODIFIED_SINCE = "If-Modified-Since";
const char* HDR_LAST_MODIFIED = "Last-Modified";
const char* HDR_CONTENT_TYPE = "Content-Type";
const char* HDR_DATE = "Date";
const char* HDR_EXPIRES = "Expires";
const char* HDR_SERVER = "Server";
const char* HDR_WWW_AUTHENTICATE = "WWW-Authenticate";
const char* HDR_SET_COOKIE = "Set-Cookie";
const char* HDR_COOKIE = "Cookie";
const char* HDR_AUTHORIZATION = "Authorization";
const char* HDR_LOCATION = "Location";

    //

const char* VAL_CONTENT_TYPE_FORM = "application/x-www-form-urlencoded";

    //

const char* COLOR_BACK = "rgb(221, 221, 255)";
const char* COLOR_TEXT = "rgb(42, 0, 100)";
const char* COLOR_PADBACK = "rgb(10, 0, 30)";
const char* COLOR_PADFRONT = "rgb(200,200,255)";
const char* COLOR_PADBORD = "back";
const char* COLOR_MENU_FRONT_ON = "rgb(255, 255, 255)";
const char* COLOR_MENU_BACK_ON = "rgb(200,150,150)";
const char* COLOR_MENU_BORDER_ON = "rgb(255,0,0)";
const char* COLOR_MENU_FRONT_OFF = "rgb(150,150,255)";
const char* COLOR_MENU_BACK_OFF = "rgb(0,0,150)";
const char* COLOR_MENU_BORDER_OFF = "rgb(0,0,100)";
const char* COLOR_MENU_FRONT_GREY = COLOR_MENU_BORDER_OFF;
const char* COLOR_MENU_BACK_GREY = "rgb(200,200,200)";
const char* COLOR_MENU_BORDER_GREY = "rgb(150,150,150)";
const char* COLOR_MENU_FRONT_HOVER_ON = "purple";
const char* COLOR_MENU_FRONT_HOVER_OFF = "white";
const char* COLOR_MENU_FRONT_HOVER_GREY = "yellow";
const char* COLOR_MENU_FRONT_ACTIVE_ON = "red";
const char* COLOR_MENU_FRONT_ACTIVE_OFF = "red";
const char* COLOR_MENU_FRONT_ACTIVE_GREY = "blue";
const char* COLOR_TOPBAR_BACK = COLOR_BACK;

const char* COLOR_ARED = "rgb(255,30,10)";
const char* RED = "red";
const char* WHITE = "white";
const char* GREEN = "rgb(0,150,80)";

const char* STATIC_PATH_ID = "st";
// STATIC_PATH_ID's length should be strictly less than the lenght of session_ID, as defined INITIAL_SESSION_ID_WIDTH in session.cpp to avoid collision with session_ID.
const char* STATIC_OBJ_LICENSING = "licensing";
const char* STATIC_LOGO = "webdar.jpg";
const char* STATIC_TITLE_LOGO = "webdar_title.jpg";
const char* STATIC_FAVICON = "favicon.jpg";

const libdar::U_I tokens_min_compr_bs = 50*1024;
