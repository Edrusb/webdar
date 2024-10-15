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

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "exceptions.hpp"


    //
#include "base64.hpp"

using namespace std;

base64::base64(const string & str)
{
    unsigned int missing = str.size() % 4;

    ch = str;
    while(missing > 0)
    {
	ch += '=';
	--missing;
    }
}


string base64::decode() const
{
    string ret = "";
    unsigned int num = ch.size() / 4;
    unsigned int res = ch.size() % 4;

    if(ch.size() == 0)
	return ret;

    if(res != 0) // missing extra bytes, should have been handled by the constructor
	throw WEBDAR_BUG;

    for(unsigned int i = 0; i < num; ++i, res += 4)
	ret += convert(ch[res], ch[res+1], ch[res+2], ch[res+3]);

    return ret;
}

string base64::convert(char a, char b, char c, char d)
{
    string ret = "";
    unsigned int v1, v2, v3, v4;
    char c1[2], c2[2], c3[2];

    v1 = value64(a);
    v2 = value64(b);
    v3 = value64(c);
    v4 = value64(d);

    c1[0] = (char)((v1 << 2) | (v2 >> 4));
    c2[0] = (char)(((v2 & 0x0F) << 4) | (v3 >> 2));
    c3[0] = (char)(((v3 & 0x03) << 6) | v4);

    c1[1] = c2[1] = c3[1] = '\0';

    ret += string(c1);
    if(c != '=')
    {
	ret += string(c2);
	if(d != '=')
	    ret += string(c3);
    }
    return ret;
}

unsigned int base64::value64(char a)
{
    if(a >= 'A' && a <= 'Z')
	return a - 'A';
    if(a >= 'a' && a <= 'z')
	return a - 'a' + 26;
    if(a >= '0' && a <= '9')
	return a - '0' + 52;
    if(a == '+')
	return 62;
    if(a == '/')
	return 63;
    if(a == '=')
	return 0;
    throw exception_range("not a valid base64 character met");
}
