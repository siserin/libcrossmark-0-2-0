
AM_CPPFLAGS = \
	$(PYTHON_INCLUDES) \
	$(DEPS_CFLAGS) \
	-I$(srcdir) \
	-I$(top_srcdir)

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

if BUILD_DOCS
all-local: copy-stamp
endif

copy-stamp:
	cp $(srcdir)/__init__.py .
	touch $@

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

CLEANFILES = \
	copy-stamp

DISTCLEANFILES = \
	crossmark-wrap.cc \
	swig-stamp
