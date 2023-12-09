########################################################################
# webdar - a web server and interface program to libdar
# Copyright (C) 2013-2023 Denis Corbin
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

SUBDIRS =
dist_noinst_DATA = Doxyfile portable_cp architecture.txt selfsigned_certificate.txt webdar.txt webdar_logo.jpg
dist_pkgdata_DATA =

if USE_DOXYGEN
DOXYGEN = @DOXYGEN_PROG@

all-local: Doxyfile.tmp

Doxyfile.tmp:
	sed -e "s%##VERSION##%@PACKAGE_VERSION@%g" -e "s%##HAS_DOT##%@HAS_DOT@%g" -e 's%##SRCDIR##%$(abs_top_srcdir)%g' -e 's%##BUILDDIR##%$(abs_top_builddir)%g' '$(srcdir)/Doxyfile' > Doxyfile.tmp
	cd '$(top_srcdir)' ; $(DOXYGEN) '$(abs_top_builddir)/doc/Doxyfile.tmp'
	if [ -d html/search ]; then chmod u+x html/search ; fi

clean-local:
	rm -rf html Doxyfile.tmp doxygen_sqlite3.db latex

install-data-hook:
	$(INSTALL) -d $(DESTDIR)$(pkgdatadir)
	'$(srcdir)/portable_cp' html $(DESTDIR)$(pkgdatadir)

uninstall-hook:
	rm -rf $(DESTDIR)$(pkgdatadir)/html
	rmdir $(DESTDIR)$(pkgdatadir) || true

else

all-local:

clean-local:

install-data-hook:

uninstall-hook:

endif


