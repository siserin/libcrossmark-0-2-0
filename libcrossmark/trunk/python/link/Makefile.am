
AM_CPPFLAGS = \
	$(PYTHON_INCLUDES) \
	$(DEPS_CFLAGS) \
	-I$(srcdir) \
	-I$(top_srcdir)

pkgpyexecdir = $(pyexecdir)/crossmark/link
pkgpyexec_PYTHON = __init__.py link.py
pkgpyexec_LTLIBRARIES = _link.la

_link_la_LDFLAGS = \
	-avoid-version \
	-module \
	-no-undefined

_link_la_LIBADD = \
	$(DEPS_LIBS) \
	$(top_builddir)/crossmark/libcrossmark-1.la

_link_la_SOURCES = \
	$(BUILT_SOURCES)

# swig-stamp is EXTRA_DIST'd so tarball builds don't require swig
swig-stamp: link.swig link.hh Makefile.am
	$(SWIG) -threads -Wall -c++ -python -directors -o link-wrap.cc $(srcdir)/$<
	touch $@

link-wrap.cc: swig-stamp

link-wrap.h: swig-stamp

link.py: swig-stamp

# when generating docs we run python tools in the build tree
all-local:
	test -f __init__.py || $(LN_S) $(srcdir)/__init__.py
	test -f link.py || $(LN_S) $(srcdir)/link.py

noinst_HEADERS = \
	link.hh

BUILT_SOURCES = \
	link-wrap.cc \
	link-wrap.h \
	link.py

EXTRA_DIST = \
	link.swig \
	$(BUILT_SOURCES) \
	swig-stamp

DISTCLEANFILES = \
	$(BUILT_SOURCES) \
	__init__.pyc \
	link.pyc \
	swig-stamp
