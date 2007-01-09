
AM_CXXFLAGS = \
	$(DEPS_CFLAGS) \
	-I $(top_srcdir)

bin_PROGRAMS = \
	xm2html \
	html2xm

xm2html_LDADD = \
	$(DEPS_LIBS) \
	$(top_builddir)/crossmark/libcrossmark-1.la

xm2html_SOURCES = \
	xm2html.cc

html2xm_LDADD = \
	$(DEPS_LIBS) \
	$(top_builddir)/crossmark/libcrossmark-1.la

html2xm_SOURCES = \
	html2xm.cc

bin_SCRIPTS = \
	xm2dox

python_PYTHON = \
	xm2dox.py

EXTRA_DIST = \
	$(pybin_DATA) \
	$(wildcard $(srcdir)/*.html) \
	$(wildcard $(srcdir)/*.xm)
