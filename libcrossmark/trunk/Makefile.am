
pkgconfdir = $(libdir)/pkgconfig
pkgconf_DATA = libcrossmark-1.pc

SUBDIRS = crossmark tools tests
if HAVE_PYTHON
SUBDIRS += python
endif
if BUILD_DOCS
SUBDIRS += docs
endif

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

CLEANFILES = \
	cl-stamp \
	copy-stamp

if BUILD_DOCS
all-local: copy-stamp
endif

clean-local: 
	if test `basename ./$(srcdir)` != `basename ./$(builddir)`; then \
		rm -f crossmark.py*; \
	fi

distclean-local:
	rm -rf *.cache *~

dist-hook: ChangeLog
	rm -rf `find $(distdir)/tests -name '\.svn'`
	rm -rf `find $(distdir)/tests -name '*~'`
	rm -rf `find $(distdir)/tests -name '*_'`

ChangeLog: cl-stamp
	if [ -d "$(srcdir)/.svn" ]; then \
		pwd=`pwd` && cd $(srcdir) && svn log -v > "$$pwd/$@"; \
	fi
	touch $<

cl-stamp:

copy-stamp:
	if test "$(srcdir)" != "$(builddir)" && \
	   test ! -f "crossmark.py"; then \
		cp $(srcdir)/crossmark.py .; \
	fi
	touch $@
