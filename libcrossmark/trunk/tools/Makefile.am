
AM_CXXFLAGS = \
	$(DEPS_CFLAGS) \
	-I $(top_srcdir)

bin_PROGRAMS = \
	cm2html \
	html2cm

cm2html_LDADD = \
	$(DEPS_LIBS) \
	$(top_builddir)/crossmark/libcrossmark-1.la

cm2html_SOURCES = \
	cm2html.cc

html2cm_LDADD = \
	$(DEPS_LIBS) \
	$(top_builddir)/crossmark/libcrossmark-1.la

html2cm_SOURCES = \
	html2cm.cc

noinst_PROGRAMS = \
	cm-scan

cm_scan_LDADD = \
	$(DEPS_LIBS) \
	$(top_builddir)/crossmark/libcrossmark-1.la

cm_scan_SOURCES = \
	cm-scan.cc

bin_SCRIPTS = \
	cm2dox

python_PYTHON = \
	cm2dox.py

EXTRA_DIST = \
	$(pybin_DATA)
