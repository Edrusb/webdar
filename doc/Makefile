all: html latex

html: Doxyfile
	if [ ! -e html ] ; then mkdir html ; fi
	doxygen Doxyfile

latex: html
	${MAKE} -C latex

clean:
	rm -rf html latex
