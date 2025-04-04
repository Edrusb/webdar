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

    // C system header files
#include "my_config.h"
extern "C"
{
#if HAVE_TIME_H
#include <time.h>
#endif
}

    // C++ system header files


    // webdar headers



    //
#include "html_datetime.hpp"

using namespace std;

const string html_datetime::changed = "html_datetime_changed";

html_datetime::html_datetime(const string & title):
    year(title, html_form_input::number, "1970", "6", ""),
    month(""),
    day(""),
    hour("At"),
    minute(":")
{
    month.add_choice("Jan","Jan");
    month.add_choice("Feb","Feb");
    month.add_choice("Mar","Mar");
    month.add_choice("Apr","Apr");
    month.add_choice("May","May");
    month.add_choice("Jun","Jun");
    month.add_choice("Jul","Jul");
    month.add_choice("Aug","Aug");
    month.add_choice("Sep","Sep");
    month.add_choice("Oct","Oct");
    month.add_choice("Nov","Nov");
    month.add_choice("Dec","Dec");

    day.add_choice(" 1"," 1");
    day.add_choice(" 2"," 2");
    day.add_choice(" 3"," 3");
    day.add_choice(" 4"," 4");
    day.add_choice(" 5"," 5");
    day.add_choice(" 6"," 6");
    day.add_choice(" 7"," 7");
    day.add_choice(" 8"," 8");
    day.add_choice(" 9"," 9");
    day.add_choice("10","10");
    day.add_choice("11","11");
    day.add_choice("12","12");
    day.add_choice("13","13");
    day.add_choice("14","14");
    day.add_choice("15","15");
    day.add_choice("16","16");
    day.add_choice("17","17");
    day.add_choice("18","18");
    day.add_choice("19","19");
    day.add_choice("20","20");
    day.add_choice("21","21");
    day.add_choice("22","22");
    day.add_choice("23","23");
    day.add_choice("24","24");
    day.add_choice("25","25");
    day.add_choice("26","26");
    day.add_choice("27","27");
    day.add_choice("28","28");
    day.add_choice("29","29");
    day.add_choice("30","30");
    day.add_choice("31","31");

    hour.add_choice("00","00");
    hour.add_choice("01","01");
    hour.add_choice("02","02");
    hour.add_choice("03","03");
    hour.add_choice("04","04");
    hour.add_choice("05","05");
    hour.add_choice("06","06");
    hour.add_choice("07","07");
    hour.add_choice("08","08");
    hour.add_choice("09","09");
    hour.add_choice("10","10");
    hour.add_choice("11","11");
    hour.add_choice("12","12");
    hour.add_choice("13","13");
    hour.add_choice("14","14");
    hour.add_choice("15","15");
    hour.add_choice("16","16");
    hour.add_choice("17","17");
    hour.add_choice("18","18");
    hour.add_choice("19","19");
    hour.add_choice("20","20");
    hour.add_choice("21","21");
    hour.add_choice("22","22");
    hour.add_choice("23","23");

    minute.add_choice("00","00");
    minute.add_choice("01","01");
    minute.add_choice("02","02");
    minute.add_choice("03","03");
    minute.add_choice("04","04");
    minute.add_choice("05","05");
    minute.add_choice("06","06");
    minute.add_choice("07","07");
    minute.add_choice("08","08");
    minute.add_choice("09","09");
    minute.add_choice("10","10");
    minute.add_choice("11","11");
    minute.add_choice("12","12");
    minute.add_choice("13","13");
    minute.add_choice("14","14");
    minute.add_choice("15","15");
    minute.add_choice("16","16");
    minute.add_choice("17","17");
    minute.add_choice("18","18");
    minute.add_choice("19","19");
    minute.add_choice("20","20");
    minute.add_choice("21","21");
    minute.add_choice("22","22");
    minute.add_choice("23","23");
    minute.add_choice("24","24");
    minute.add_choice("25","25");
    minute.add_choice("28","28");
    minute.add_choice("27","27");
    minute.add_choice("28","28");
    minute.add_choice("29","29");
    minute.add_choice("30","30");
    minute.add_choice("31","31");
    minute.add_choice("32","32");
    minute.add_choice("33","33");
    minute.add_choice("34","34");
    minute.add_choice("35","35");
    minute.add_choice("36","36");
    minute.add_choice("37","37");
    minute.add_choice("38","38");
    minute.add_choice("39","39");
    minute.add_choice("40","40");
    minute.add_choice("41","41");
    minute.add_choice("42","42");
    minute.add_choice("43","43");
    minute.add_choice("44","44");
    minute.add_choice("45","45");
    minute.add_choice("46","46");
    minute.add_choice("47","47");
    minute.add_choice("48","48");
    minute.add_choice("49","49");
    minute.add_choice("50","50");
    minute.add_choice("51","51");
    minute.add_choice("52","52");
    minute.add_choice("53","53");
    minute.add_choice("54","54");
    minute.add_choice("55","55");
    minute.add_choice("56","56");
    minute.add_choice("57","57");
    minute.add_choice("58","58");
    minute.add_choice("59","59");

    year.set_min_only(1970);

    year.set_no_CR();
    month.set_no_CR();
    day.set_no_CR();
    hour.set_no_CR();


	// adoption tree

    adopt(&year);
    adopt(&month);
    adopt(&day);
    adopt(&hour);
    adopt(&minute);

	// events

    year.record_actor_on_event(this, html_form_input::changed);
    month.record_actor_on_event(this, html_form_select::changed);
    day.record_actor_on_event(this, html_form_select::changed);
    hour.record_actor_on_event(this, html_form_select::changed);
    minute.record_actor_on_event(this, html_form_select::changed);
    register_name(changed);
}

libdar::infinint html_datetime::get_value() const
{
    time_t tmp_ret;
    struct tm splitted;
    unsigned int tmp = webdar_tools_convert_to_int(year.get_value());

    if(tmp < 1900)
	throw WEBDAR_BUG;
    splitted.tm_sec = 0;
    splitted.tm_min = minute.get_selected_num();
    splitted.tm_hour = hour.get_selected_num();
    splitted.tm_mday = day.get_selected_num() + 1; // tm_mday starts at 1 unlike the other fields...
    splitted.tm_mon = month.get_selected_num();
    splitted.tm_year = tmp - 1900;
    splitted.tm_wday = 0;
    splitted.tm_yday = 0;
    splitted.tm_isdst = 0; // never consider daylight saving, but always and only GMT

    tmp_ret = timegm(&splitted);
    if(tmp_ret == -1)
	throw WEBDAR_BUG;
    if(tmp_ret < 0)
	throw WEBDAR_BUG;
	// not supporting dates before year 1970 (a year of great things ;)
	//timegm() should never be return negative dates but just in case

    return libdar::infinint(tmp_ret);
}

void html_datetime::set_value(const libdar::infinint & val)
{
    time_t tmp;
    struct tm splitted;
    struct tm *ret;

    tmp = webdar_tools_convert_from_infinint<time_t>(val,
						     string("value given from libdar is not possible to be stored as time_t system type"));
    ret = gmtime_r(&tmp, &splitted);
    if(ret == nullptr)
	throw WEBDAR_BUG; // system failed representing this date in splitted fields
    update_from(splitted);
}

void html_datetime::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed
       || event_name == html_form_select::changed)
	act(changed);
    else
	throw WEBDAR_BUG;
}


string html_datetime::inherited_get_body_part(const chemin & path,
					      const request & req)
{
    string ret;
    bool err = false;
    int annees;

    if(get_no_CR())
	minute.set_no_CR();
    ret = get_body_part_from_all_children(path, req);

    try
    {
	annees = webdar_tools_convert_to_int(year.get_value());
    }
    catch(exception_range & e)
    {
	err = true;
    }

    if(err || annees < 70)
    {
	year.set_value("1970");
	if(req.get_method() == "POST")
	{
	    request tmpreq = req;
	    tmpreq.change_method("GET");
	    ret = get_body_part_from_all_children(path, tmpreq);
	}
	else
	    ret = get_body_part_from_all_children(path, req);
    }

    return ret;
}

void html_datetime::update_from(struct tm val)
{
    unsigned int year_int = val.tm_year + 1900;

    if(year_int < 1970) // sanity check, should not occur
    {
	val.tm_min = 0;
	val.tm_hour = 0;
	val.tm_mday = 0;
	val.tm_mon = 0;
	year_int = 1970;
    }

    minute.set_selected_num(val.tm_min);
    hour.set_selected_num(val.tm_hour);
    day.set_selected_num(val.tm_mday - 1); // day starts at 1 unlike other fields
    month.set_selected_num(val.tm_mon);
    year.set_value(webdar_tools_convert_to_string(year_int));
}
