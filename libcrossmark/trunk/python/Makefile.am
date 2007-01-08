
AM_CPPFLAGS = \
	$(PYTHON_INCLUDES) \
	$(DEPS_CFLAGS) \
	-I$(srcdir) \
	-I$(top_srcdir)

pkgpyexecdir = @pyexecdir@
pkgpyexec_DATA = crossmark.py _crossmark.so
# TODO this gets installed, but noinst_LTLIBRARIES doesn't create the .so for some reason
pkgpyexec_LTLIBRARIES = libcrossmark.la

libcrossmark_la_LDFLAGS = \
	-avoid-version
	-module -no-undefined

libcrossmark_la_LIBADD = \
	$(DEPS_LIBS) \
	../crossmark/libcrossmark-1.la

libcrossmark_la_SOURCES = \
	crossmark-wrap.cc

crossmark-wrap.cc: crossmark.swig document.hh parser.hh Makefile.am
	$(SWIG) -threads -Wall -c++ -python -directors -o $@ $(srcdir)/crossmark.swig

noinst_HEADERS = \
	document.hh \
	parser.hh

BUILT_SOURCES = \
	crossmark-wrap.cc

EXTRA_DIST = \
	crossmark.swig \
	crossmark.py

CLEANFILES = \
	_crossmark.so

DISTCLEANFILES = \
	crossmark-wrap.cc \
	$(pkgpyexec_DATA)

_crossmark.so: libcrossmark.la
	cp .libs/libcrossmark.so $@
