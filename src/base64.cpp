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


string base64::decode(const std::string & str) const
{
    string ret = "";
    string tmp = str;
    unsigned int num;
    unsigned int res;
    unsigned int len;

    complement(tmp);
    len = tmp.size();
    res = len % 4;

    if(len == 0)
	return ret;

    if(res != 0) // missing extra bytes, should have been handled by the constructor
	throw WEBDAR_BUG;

    for(unsigned int i = 0; i < len; i += 4)
	ret += convert(tmp[i], tmp[i+1], tmp[i+2], tmp[i+3]);

    return ret;
}

void base64::small_decode(const encoded_block & b64, decoded_block & out) const
{
    string tmp = convert(b64[0], b64[1], b64[2], b64[3]);

    if(tmp.size() != 3)
	throw WEBDAR_BUG;

    for(unsigned int i = 0; i < 3; ++i)
	out[i] = tmp[i];
}

void base64::small_encode(unsigned int num_bytes, const decoded_block & bin, encoded_block & out) const
{
    string tmp = unconvert(num_bytes, bin[0], bin[1], bin[2]);

    if(tmp.size() != 4)
	throw WEBDAR_BUG;

    for(unsigned int i = 0; i < 4; ++i)
	out[i] = tmp[i];
}


std::string base64::encode(const std::string & str) const
{
    string ret = "";
    unsigned int num = str.size() / 3;
    unsigned int res = str.size() % 3;

    if(str.size() == 0)
	return str;

    for(unsigned int i = 0; i < num; i += 3)
	ret += unconvert(3, str[i], str[i+1], str[i+2]);

    num *= 3;

    switch(res)
    {
    case 0:
	break;
    case 1:
	ret += unconvert(1, str[num], 0, 0);
	break;
    case 2:
	ret += unconvert(2, str[num], str[num+1], 0);
	break;
    default:
	throw WEBDAR_BUG;
    }

    return ret;
}

string base64::convert(char a, char b, char c, char d)
{
    string ret = "";
    unsigned int v1, v2, v3, v4;
    char c1, c2, c3;

    v1 = value64(a);
    v2 = value64(b);
    v3 = value64(c);
    v4 = value64(d);

    c1 = (char)((v1 << 2) + (v2 >> 4));
    c2 = (char)(((v2 & 0x0F) << 4) + (v3 >> 2));
    c3 = (char)(((v3 & 0x03) << 6) + v4);

    ret.push_back(c1);
    ret.push_back(c2);
    ret.push_back(c3);

    if(ret.size() != 3)
	throw WEBDAR_BUG;

    return ret;
}

string base64::unconvert(int num_char, unsigned char a, unsigned char b, unsigned char c)
{
    string ret = "";
    unsigned char c1[2], c2[2], c3[2], c4[2];

    switch(num_char)
    {
    case 0:
	throw WEBDAR_BUG;
    case 1:
	b = c = 0;
	break;
    case 2:
	c = 0;
	break;
    case 3:
	break;
    default:
	throw WEBDAR_BUG;
    }

    c1[0] = a >> 2;
    c2[0] = ((a & 0x03) << 4) + (b >> 4);
    c3[0] = ((b & 0x0F) << 2) + (c >> 6);
    c4[0] = c & 0x3F;

    c1[0] = to_value64(c1[0]);
    c2[0] = to_value64(c2[0]);
    c3[0] = to_value64(c3[0]);
    c4[0] = to_value64(c4[0]);

	// padding

    switch(num_char)
    {
    case 1:
	c3[0] = '=';
	c4[0] = '=';
	break;
    case 2:
	c4[0] = '=';
	break;
    case 3:
	break;
    default:
	throw WEBDAR_BUG;
    }

	// building the resulting string portion

    c1[1] = c2[1] = c3[1] = c4[1] = '\0';
    ret += string((char*)c1) + string((char*)c2) + string((char*)c3) + string((char*)c4);

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

char base64::to_value64(unsigned int val)
{
    if(val < 26)
	return 'A' + val;
    if(val < 52) // val is in [26..52[
	return 'a' + (val - 26);
    if(val < 62) // val is in [52..62[
	return '0' + (val - 52);
    switch(val)
    {
    case 62:
	return '+';
    case 63:
	return '/';
    default:
	throw WEBDAR_BUG;
    }
}

void base64::complement(string & str)
{
    unsigned int alone = str.size() % 4;
    unsigned int missing = alone != 0 ? 4 - alone : 0;

    while(missing > 0)
    {
	str += '=';
	--missing;
    }
}

