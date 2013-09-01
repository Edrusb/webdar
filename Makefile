PREFIX=/usr/local
DESTDIR=
VERSION=0.0.1

export

all:
	${MAKE} -C src all

clean:
	${MAKE} -C src clean

install:
	mkdir -p ${DESTDIR}${PREFIX}/bin
	mkdir -p ${DESTDIR}${PREFIX}/share/man/man1
	cp src/webdar ${DESTDIR}${PREFIX}/bin
	cp man/webdar.1 ${DESTDIR}${PREFIX}/share/man/man1

install-strip: install
	strip ${DESTDIR}${PREFIX}/bin/webdar

uninstall:
	rm ${DESTDIR}${PREFIX}/bin/webdar
	rm ${DESTDIR}${PREFIX}/share/man/man1/webdar.1

dist:
	mkdir -p webdar-${VERSION}/src webdar-${VERSION}/man webdar-${VERSION}/doc
	cp src/* webdar-${VERSION}/src
	cp man/* webdar-${VERSION}/man
	cp doc/* webdar-${VERSION}/doc
	cp README COPYING INSTALL TODO Makefile webdar-${VERSION}
	tar -czf webdar-${VERSION}.tar.gz webdar-${VERSION}
	rm -rf webdar-${VERSION}

