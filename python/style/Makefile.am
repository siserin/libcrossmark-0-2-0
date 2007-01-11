
AM_CPPFLAGS = \
	$(PYTHON_INCLUDES) \
	$(DEPS_CFLAGS) \
	-I$(srcdir) \
	-I$(top_srcdir)

pkgpyexecdir = $(pyexecdir)/crossmark/style
pkgpyexec_PYTHON = __init__.py style.py
pkgpyexec_LTLIBRARIES = _style.la

_style_la_LDFLAGS = \
	-avoid-version \
	-module \
	-no-undefined

_style_la_LIBADD = \
	$(DEPS_LIBS) \
	$(top_builddir)/crossmark/libcrossmark-1.la

_style_la_SOURCES = \
	$(BUILT_SOURCES)

# swig-stamp is EXTRA_DIST'd so tarball builds don't require swig
swig-stamp: style.swig style.hh Makefile.am
	$(SWIG) -threads -Wall -c++ -python -directors -o style-wrap.cc $(srcdir)/$<
	touch $@

style-wrap.cc: swig-stamp

style-wrap.h: swig-stamp

style.py: swig-stamp

if BUILD_DOCS
all-local: copy-stamp
endif

clean-local: 
	if test `basename ./$(srcdir)` != `basename ./$(builddir)`; then \
		rm -f __init__.py*; \
		rm -f style.py*; \
	fi

copy-stamp:
	if test `basename ./$(srcdir)` != `basename ./$(builddir)`; then \
		cp $(srcdir)/__init__.py .; \
		if test -f "$(srcdir)/style.py"; then \
			cp $(srcdir)/style.py .; \
		fi; \
	fi
	touch $@

noinst_HEADERS = \
	style.hh

BUILT_SOURCES = \
	style-wrap.cc \
	style-wrap.h \
	style.py

EXTRA_DIST = \
	style.swig \
	$(BUILT_SOURCES) \
	swig-stamp

CLEANFILES = \
	copy-stamp

DISTCLEANFILES = \
	style-wrap.cc \
	swig-stamp
