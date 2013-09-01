PREFIX=/usr/local
DESTDIR=
VERSION=0.0.1

all:
	cd src ; make all

clean:
	cd src ; make clean

install:
	cp src/webdar ${DESTDIR}${PREFIX}/bin
	cp man/webdar.1 ${DESTDIR}${PREFIX}/man/man.1

uninstall:
	rm ${DESTDIR}${PREFIX}/bin/webdar
	rm ${DESTDIR}${PREFIX}/man/man.1/webdar.1

dist:
	mkdir -p webdar-${VERSION}/src webdar-${VERSION}/man webdar-${VERSION}/doc
	cp src/* webdar-${VERSION}/src
	cp webdar/* webdar-${VERSION}/webdar
	cp doc/* webdar-${VERSION}/doc
	cp README COPYING INSTALL TODO Makefile webdar-${VERSION}
	tar -czf webdar-${VERSION}.tar.gz webdar-${VERSION}
	rm -rf webdar-${VERSION}

