
AM_CXXFLAGS = \
	$(DEPS_CFLAGS) \
	-I $(top_srcdir)

noinst_PROGRAMS = cm-scan cm-parse

cm_scan_LDADD = \
	$(DEPS_LIBS) \
	../crossmark/libcrossmark-1.la

cm_scan_SOURCES = cm-scan.cc

cm_parse_LDADD = \
	$(DEPS_LIBS) \
	../crossmark/libcrossmark-1.la

cm_parse_SOURCES = cm-parse.cc
