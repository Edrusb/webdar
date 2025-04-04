########################################################################
# webdar - a web server and interface program to libdar
# Copyright (C) 2013-2025 Denis Corbin
#
# This file is part of Webdar
#
#  Webdar is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Webdar is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
#
#----
#  to contact the author: dar.linux@free.fr
########################################################################

SUBDIRS = src doc po
dist_noinst_DATA = INSTALL README TODO AUTHORS COPYING ChangeLog

CPPCHECKDIR=./cppcheckbuilddir

cppcheck:
	@if cppcheck --help > /dev/null ; then \
	( mkdir -p $(CPPCHECKDIR) && cppcheck --force --file-filter="*.?pp" -isrc/testing -isrc/python --cppcheck-build-dir=$(CPPCHECKDIR) `pwd` ) || exit 1 ; \
	else \
	echo "cppcheck not present, aborting" || exit 1 ; \
	fi

clean-local:
	rm -rf $(CPPCHECKDIR)

