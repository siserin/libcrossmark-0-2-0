
AM_CPPFLAGS = \
	$(PYTHON_INCLUDES) \
	$(DEPS_CFLAGS) \
	-I$(srcdir) \
	-I$(top_srcdir)

pkgpyexecdir = @pyexecdir@
pkgpyexec_DATA = crossmark.py
pkgpyexec_LTLIBRARIES = _crossmark.la

_crossmark_la_LDFLAGS = \
	-module \
	-avoid-version

_crossmark_la_LIBADD = \
	$(DEPS_LIBS) \
	../crossmark/libcrossmark-1.la

_crossmark_la_SOURCES = \
	crossmark_wrap.cxx

crossmark_wrap.cxx: crossmark.swig
	$(SWIG) -c++ -python $(srcdir)/crossmark.swig

noinst_HEADERS = \
	document.hh

BUILT_SOURCES = \
	crossmark_wrap.cxx

EXTRA_DIST = \
	crossmark.swig \
	$(pkgpyexec_DATA)

DISTCLEANFILES = \
	crossmark_wrap.cxx \
	crossmark.py
