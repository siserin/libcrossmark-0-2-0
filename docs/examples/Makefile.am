
AM_CXXFLAGS = \
	$(DEPS_CFLAGS) \
	-I $(top_srcdir)

noinst_PROGRAMS = \
	xm2dox

xm2dox_LDADD = \
	$(DEPS_LIBS) \
	$(top_builddir)/crossmark/libcrossmark-1.la

xm2dox_SOURCES = xm2dox.cc

EXTRA_DIST = \
	parse.py \
	write.py
