
AM_CPPFLAGS = \
	$(PYTHON_INCLUDES) \
	$(DEPS_CFLAGS) \
	-I$(srcdir) \
	-I$(top_srcdir)

python_PYTHON = crossmark.py
pyexec_DATA = _crossmark.so
# TODO this gets installed, but noinst_LTLIBRARIES doesn't create the .so for 
# some reason. Maybe rather just symlink _crossmark.so
pyexec_LTLIBRARIES = libcrossmark.la

libcrossmark_la_LDFLAGS = \
	-avoid-version
	-module -no-undefined

libcrossmark_la_LIBADD = \
	$(DEPS_LIBS) \
	../crossmark/libcrossmark-1.la

libcrossmark_la_SOURCES = \
	$(BUILT_SOURCES)

crossmark-wrap.cc: crossmark.swig document.hh parser.hh writer.hh Makefile.am
	$(SWIG) -threads -Wall -c++ -python -directors -o $@ $(srcdir)/crossmark.swig

crossmark-wrap.h: crossmark-wrap.cc

crossmark.py: crossmark-wrap.cc

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
	$(BUILT_SOURCES)

CLEANFILES = \
	_crossmark.so

DISTCLEANFILES = \
	crossmark-wrap.cc \
	$(pkgpyexec_DATA)

_crossmark.so: libcrossmark.la
	cp .libs/libcrossmark.so $@
