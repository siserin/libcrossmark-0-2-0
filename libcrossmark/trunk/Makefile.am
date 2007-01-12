
SUBDIRS = crossmark tools tests
if HAVE_PYTHON
SUBDIRS += python
endif
if BUILD_DOCS
SUBDIRS += docs
endif

pkgconfdir = $(libdir)/pkgconfig
pkgconf_DATA = \
	libcrossmark-1.pc

docdir = $(datadir)/doc/$(PACKAGE)-$(VERSION)
doc_DATA = \
	AUTHORS			\
	COPYING			\
	HACKING			\
	INSTALL			\
	README

EXTRA_DIST = \
	$(doc_DATA)		\
	ChangeLog		\
	autogen.sh		\
	crossmark.py		\
	libcrossmark-1.pc.in	\
	python.m4

DISTCLEANFILES = \
	crossmark.pyc \
	cl-stamp

dist-hook: ChangeLog
	rm -rf `find $(distdir)/tests -name '\.svn'`
	rm -rf `find $(distdir)/tests -name '*~'`
	rm -rf `find $(distdir)/tests -name '*_'`

distclean-local:
	rm -rf *.cache *~
	if test `basename ./$(srcdir)` != `basename ./$(builddir)`; then \
		test -f crossmark.py && rm -f crossmark.py; \
	fi

ChangeLog: cl-stamp
	if [ -d "$(srcdir)/.svn" ]; then \
		pwd=`pwd` && cd $(srcdir) && svn log -v > "$$pwd/$@"; \
	fi
	touch $<

cl-stamp:
