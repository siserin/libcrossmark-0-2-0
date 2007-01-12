
SUBDIRS = block style

AM_CPPFLAGS = \
	$(PYTHON_INCLUDES) \
	$(DEPS_CFLAGS) \
	-I$(srcdir) \
	-I$(top_srcdir)

pkgpyexecdir = $(pyexecdir)/crossmark
pkgpyexec_PYTHON = __init__.py crossmark.py
pkgpyexec_LTLIBRARIES = _crossmark.la

_crossmark_la_LDFLAGS = \
	-avoid-version \
	-module \
	-no-undefined

_crossmark_la_LIBADD = \
	$(DEPS_LIBS) \
	$(top_builddir)/crossmark/libcrossmark-1.la

_crossmark_la_SOURCES = \
	$(BUILT_SOURCES)

# swig-stamp is EXTRA_DIST'd so tarball builds don't require swig
swig-stamp: crossmark.swig document.hh parser.hh writer.hh Makefile.am
	$(SWIG) -threads -Wall -c++ -python -directors -o crossmark-wrap.cc $(srcdir)/$<
	touch $@

crossmark-wrap.cc: swig-stamp

crossmark-wrap.h: swig-stamp

crossmark.py: swig-stamp

# when generating docs we run python tools in the build tree
all-local:
	test -f __init__.py || $(LN_S) $(srcdir)/__init__.py
	test -f crossmark.py || $(LN_S) $(srcdir)/crossmark.py

noinst_HEADERS = \
	document.hh \
	parser.hh \
	writer.hh

BUILT_SOURCES = \
	crossmark-wrap.cc \
	crossmark-wrap.h \
	crossmark.py

EXTRA_DIST = \
	crossmark.swig \
	$(BUILT_SOURCES) \
	swig-stamp

DISTCLEANFILES = \
	$(BUILT_SOURCES) \
	__init__.pyc \
	crossmark.pyc \
	swig-stamp
