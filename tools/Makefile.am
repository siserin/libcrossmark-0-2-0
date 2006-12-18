
AM_CXXFLAGS = \
	$(DEPS_CFLAGS) \
	-I $(top_srcdir)



bin_PROGRAMS = cm2html

cm2html_LDADD = \
	$(DEPS_LIBS) \
	../crossmark/libcrossmark-1.la

cm2html_SOURCES = cm2html.cc



noinst_PROGRAMS = cm-scan

cm_scan_LDADD = \
	$(DEPS_LIBS) \
	../crossmark/libcrossmark-1.la

cm_scan_SOURCES = cm-scan.cc
