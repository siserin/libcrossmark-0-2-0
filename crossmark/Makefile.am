if PLATFORM_WIN32
no_undefined = -no-undefined
else
if PLATFORM_DARWIN
no_undefined = -no-undefined
else
no_undefined =
endif
endif

if OS_WIN32
version_info = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) -avoid-version

install-libtool-import-lib:
	$(INSTALL) .libs/libcrossmark*-1.dll.a $(DESTDIR)$(libdir)

uninstall-libtool-import-lib:
	-rm $(DESTDIR)$(libdir)/libcrossmark*-1.dll.a

else
version_info = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)
install-libtool-import-lib:
uninstall-libtool-import-lib:
endif

lib_LTLIBRARIES = libcrossmark-1.la
libcrossmark_1_includedir = $(includedir)/libcrossmark-1/crossmark
libcrossmark_1_include_HEADERS = crossmark.h

INCLUDES = -I$(srcdir)

AM_CXXFLAGS = $(LIBCROSSMARK_CXXFLAGS) $(DEBUG_CXXFLAGS)

#libcrossmark_1_la_LIBADD = @LIBCROSSMARK_WIN32_RESOURCE@
#libcrossmark_1_la_DEPENDENCIES = @LIBCROSSMARK_WIN32_RESOURCE@

libcrossmark_1_la_CXXFLAGS = $(DEPS_CFLAGS)

libcrossmark_1_la_LDFLAGS = \
	$(DEPS_LIBS) \
	$(version_info) \
	-export-dynamic \
	$(no_undefined)

libcrossmark_1_la_SOURCES = \
	sink.cpp \
	writer.cpp

if OS_WIN32
@LIBCROSSMARK_WIN32_RESOURCE@ : libcrossmark.rc
	$(top_srcdir)/build/win32/lt-compile-resource libcrossmark.rc @LIBCROSSMARK_WIN32_RESOURCE@
endif


install-data-local: install-libtool-import-lib

uninstall-local: uninstall-libtool-import-lib

EXTRA_DIST = \
	libcrossmark.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	libcrossmark.rc 
