
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

# when generating docs we run python tools in the build tree
all-local:
	test -f __init__.py || $(LN_S) $(srcdir)/__init__.py
	test -f style.py || $(LN_S) $(srcdir)/style.py

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

DISTCLEANFILES = \
	$(BUILT_SOURCES) \
	__init__.pyc \
	style.pyc \
	swig-stamp
