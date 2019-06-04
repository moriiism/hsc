LIBS = hsclib
BINS = step

SUBDIRS = $(LIBS) $(BINS)  doc  setup

all: libs bins

libs : 
	for dir in ${LIBS}; do \
	(cd $$dir; ${MAKE} all); \
	done

bins: libs
	for dir in ${BINS}; do \
	(cd $$dir; ${MAKE} all); \
	done

install: all
	for dir in ${BINS}; do \
	(cd $$dir; ${MAKE} install); \
	done

clean:
	for dir in ${SUBDIRS}; do \
	(cd $$dir; ${MAKE} clean); \
	done

cleaner: clean
	for dir in ${SUBDIRS}; do \
	(cd $$dir; ${MAKE} cleaner); \
	done

allclean: cleaner

uninstall: cleaner
	-rm -fr lib
