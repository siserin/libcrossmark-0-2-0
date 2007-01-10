
AM_CPPFLAGS = \
	$(DEBUG_CXXFLAGS) \
	-I$(top_srcdir)

noinst_LTLIBRARIES = libstream.la

libstream_la_CXXFLAGS = $(DEPS_CFLAGS)

libstream_la_SOURCES = \
	stdio-stream.cc \
	stdio-stream-private.hh \
	stream.cc \
	stream-private.hh

gsf_sources = \
	gsf-stream.cc \
	gsf-stream-private.hh

if HAVE_LIBGSF
libstream_la_SOURCES += $(gsf_sources)
endif

headersdir = $(includedir)/libcrossmark-1/crossmark/stream
headers_DATA = \
	stream.hh

EXTRA_DIST = \
	$(headers_DATA)
