
AM_CXXFLAGS = \
	$(DEPS_CFLAGS) \
	-I $(top_srcdir)

noinst_PROGRAMS = \
	xm-scan

xm_scan_LDADD = \
	$(DEPS_LIBS) \
	$(top_builddir)/crossmark/libcrossmark-1.la

xm_scan_SOURCES = \
	xm-scan.cc
