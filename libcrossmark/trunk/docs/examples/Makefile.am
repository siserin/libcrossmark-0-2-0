
AM_CXXFLAGS = \
	$(DEPS_CFLAGS) \
	-I $(top_srcdir)

noinst_PROGRAMS = \
	cm2dox

cm2dox_LDADD = \
	$(DEPS_LIBS) \
	$(top_builddir)/crossmark/libcrossmark-1.la

cm2dox_SOURCES = cm2dox.cc

EXTRA_DIST = \
	parse.py \
	write.py
